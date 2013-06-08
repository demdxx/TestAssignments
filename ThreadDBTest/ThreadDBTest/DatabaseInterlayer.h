//
//  DatabaseInterlayer.h
//  ThreadDBTest
//
//  Created by Dmitry Ponomarev on 6/6/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

// Здесь мне больше импонирует использование дерективы #pragma once

#define DB_SINCHRONIZE 1
#define DB_SYNC_EVERY 3 // Seconds

#ifndef __ThreadDBTest__DatabaseInterlayer__
#define __ThreadDBTest__DatabaseInterlayer__

#include <string>
//#include <unordered_map> // awailable in clib llvm
#include <boost/unordered_map.hpp>

#include "cppconn/driver.h"

#include "DatabaseCache.h"

class DatabaseInterlayer
{
private:
    DatabaseCache *_cache;
    sql::Connection *_connection;
    std::string _table;

#if DB_SINCHRONIZE
    pthread_t _thread;
#endif

public:
    DatabaseInterlayer(const char *table, DatabaseCache *cache)
        : _table(table)
        , _cache(cache)
        , _connection(NULL) {}
    virtual ~DatabaseInterlayer(void) {
        disconnect();
    }

    bool connect(const char *host, const char *login, const char *password, const char *scheme=NULL);
    
    /**
     * Disconnect DB
     */
    inline void disconnect(void) {
//#if DB_SINCHRONIZE
//        stopTimer();
//#endif
        if (_connection) {
            delete _connection;
            _connection = NULL;
        }
    }
    
    inline bool isConnected(void) {
        return NULL!=_connection; // && !_connection->isClosed();
    }
    
    inline void setScheme(const char *scheme) {
        if (_connection) {
            _connection->setSchema(scheme);
        }
    }
    
    inline void setTable(const char *table) {
        _table = table;
    }
    
    inline const char* getTable(void) {
        return _table.c_str();
    }

public: // Database actions
    bool putToDB(const char *key, const char *value);
    bool getFromDB(const char *key, std::string& value);
//#if DB_SINCHRONIZE
//    bool startTimer(void);
//    void stopTimer(void);
//public:
//    bool synchronize(void);
//#endif

public: // Public Actions
    bool getRecord(const char *key, Record *&record);
    bool setRecord(const char *key, const char *value);
    
    // Для корректной очистки данных нужно заблокировать
    // доступ всех потоков к операциям с кэшем
    void clearCache(void);
};

#endif /* defined(__ThreadDBTest__DatabaseInterlayer__) */
