//
//  DatabaseInterlayer.cpp
//  ThreadDBTest
//
//  Created by Dmitry Ponomarev on 6/6/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

/**
  Закомментированна реализация с встроенным map для хранения кэша
  Кэш вынесен в отдельный класс
 */

#include <iostream>

#include "DatabaseInterlayer.h"

#include "cppconn/driver.h"
#include "cppconn/exception.h"
#include "cppconn/resultset.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"

#define SQL_TRY try {
#define SQL_CATCH \
    } catch (sql::SQLException &e) { \
        std::cout << "# ERR: SQLException in " << __FILE__; \
        std::cout << "(" << __FUNCTION__ << ") on line " \
                  << __LINE__ << std::endl; \
        std::cout << "# ERR: " << e.what(); \
        std::cout << " (MySQL error code: " << e.getErrorCode(); \
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl; \
        return false; \
    }

/** ****************************************************************************
 * Database actions
 */
bool DatabaseInterlayer::connect(const char *host, const char *login, const char *password, const char *scheme) {
    if (isConnected()) {
        return false;
    }

    SQL_TRY
        /* Create a connection */
        sql::Driver *driver = get_driver_instance();
        _connection = driver->connect(host, login, password);
        /* Connect to the MySQL test database */
        if (NULL!=scheme) {
            _connection->setSchema(scheme);
        }
        //std::cout << "Driver: " << driver->getName() << std::endl;
//#if DB_SINCHRONIZE
//        startTimer();
//#endif
    SQL_CATCH
    return true;
}

bool DatabaseInterlayer::putToDB(const char *key, const char *value) {
    if (!isConnected()) {
        return false;
    }

    sql::PreparedStatement *pstmt;
    std::string q = "INSERT INTO ";
    q += _table;
    q += " (`key`, `data`) VALUES (?, ?)"
         " ON DUPLICATE KEY UPDATE `data`=VALUES(`data`);";
    
    SQL_TRY
        /* '?' is the supported placeholder syntax */
        pstmt = _connection->prepareStatement(q);
        pstmt->setString(1, key);
        pstmt->setString(2, value);
        pstmt->executeUpdate();
        delete pstmt;
    SQL_CATCH
    
    return true;
}

bool DatabaseInterlayer::getFromDB(const char *key, std::string& value) {
    if (!isConnected()) {
        return false;
    }

    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;
    bool result = false;
    std::string q = "SELECT `data` FROM ";
    q += _table;
    q += " WHERE `key`=?;";
    
    SQL_TRY
        /* '?' is the supported placeholder syntax */
        pstmt = _connection->prepareStatement(q);
        pstmt->setString(1, key);
        res = pstmt->executeQuery();
        
        if (res && res->next()) {
            value = res->getString(1);
            result = true;
        }
        
        delete res;
        delete pstmt;
    SQL_CATCH
    
    return result;
}

//#if DB_SINCHRONIZE
//void* _timer(void *attr) {
//    for (;;) {
//        sleep(DB_SYNC_EVERY);
//        ((DatabaseInterlayer*)attr)->synchronize();
//    }
//}
//
//bool DatabaseInterlayer::startTimer(void)
//{
//    return 0==pthread_create(&_thread, NULL, _timer, (void*)this);
//}
//
//void DatabaseInterlayer::stopTimer(void) {
//    pthread_cancel(_thread);
//}
//
//bool DatabaseInterlayer::synchronize(void) {
//    // Здесь нужно поставить проверку синхронизируется уже или нет
//    std::cout << "synchronize" << std::endl;
//    if (!_connection) {
//        return false;
//    }
//    
//    DatabaseCache::cache_map *cache = _cache->getMap();
//    if (cache->size()>0) {
//        DatabaseCache::cache_iterator it = cache->begin();
//        for (;it!=cache->end();it++) {
//            if (it->second.isModified() && it->second.lock()) {
//                if (!putToDB(it->first.c_str(), it->second.getData())) {
//                    return false;
//                }
//                it->second.unlock();
//            }
//        }
//    }
//    return true;
//}
//#endif

/** ****************************************************************************
 * Public actions
 */

/**
 * Получить значение из кэша если есть.
 * Если нет получть из базы и закэшировать.
 * Если нет в базе то добавить и закэшировать
 *
 * @param key
 * @param value
 * @return bool
 */
bool DatabaseInterlayer::getRecord(const char *key, Record *&record) {
//    cache_iterator it = _cache.find(key);
//    if (it==_cache.end()) {
//        std::string value;
//#if !DB_SINCHRONIZE
//        if (!getFromDB(key, value) && !putToDB(key, "")) {
//            return false;
//        }
//#else
//        if (getFromDB(key, value)) {
//            _cache.insert(std::pair<std::string, Record>(key, Record(value.c_str())));
//        }
//#endif
//        else {
//            _cache.insert(std::pair<std::string, Record>(key, Record()));
//        }
//        it = _cache.find(key);
//    }
//    record = &it->second;

    if (NULL==_cache) {
        return false;
    }
    else if (!_cache->getRecord(key, record)) {
        std::string value;
#if !DB_SINCHRONIZE
        if (!getFromDB(key, value) && !putToDB(key, "")) {
            return false;
        }
#else
        if (getFromDB(key, value)) {
            _cache->setRecord(key, value.c_str());
        }
#endif
        else {
            _cache->setRecord(key, NULL);
        }
        _cache->getRecord(key, record);
    }
    
    return true;
}

/**
 * Отправить значение в базу данных if !DB_SINCHRONIZE
 * Если значение вкэше есть то обновляем его
 * 
 * @param key
 * @param value
 * @return bool
 */
bool DatabaseInterlayer::setRecord(const char *key, const char *value) {
#if !DB_SINCHRONIZE
    if (!putToDB(key, value)) {
        return false;
    }
#endif
//    cache_iterator it = _cache.find(key);
//    if (it!=_cache.end()) {
//        it->second.setData(value);
//    } else {
//        _cache.insert(std::pair<std::string, Record>(key, Record(value)));
//    }
    _cache->setRecord(key, value);
    return true;
}

/**
 * Чистим кэш
 */
void DatabaseInterlayer::clearCache(void) {
    _cache->clear();
}

