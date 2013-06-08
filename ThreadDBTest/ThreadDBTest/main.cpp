//
//  main.cpp
//  ThreadDBTest
//
//  Created by Dmitry Ponomarev on 6/6/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

/**
Имеем базу данных с единственной таблицей:
records
{
    key : varchar
    data : text
}
Имеем многопоточное приложение, которое работает с записями в этой таблице.
Необходимо реализовать класс-прослойку(кэш) между приложением и базой данных со следующими свойствами:
 - Одновременно с одной записью в кэше должен работать только один поток в приложении.
 - Несколько потоков должны иметь возможность работать с несколькими разными записями параллельно.
 - Прослойка должна кэшировать записи и сохранять изменения в базу данных с заданной периодичностью.
 - Если поток запросил запись, которая занята другим потоком, то он должен ждать записи некоторое время, по истечению которого выкинуть исключение.
 - Если поток запросил запись, которой нет в кэше, то кэш должен подгрузить её из базы.
 - Если поток запросил запись, которой нет в базе данных, то кэш должен создать пустую запись в базе с запрашиваемым ключом.

CREATE TABLE IF NOT EXISTS records (
   `key` VARCHAR(32) NOT NULL UNIQUE
 , `data` TEXT NOT NULL
 , PRIMARY KEY (`key`)
) ENGINE = MyISAM;

*/

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

//#include <boost/thread/mutex.hpp>
#import <time.h>

#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <string>

#include "DatabaseInterlayer.h"
#include "MemoryDatabaseCache.h"

#define THREADS_COUNT 100

#define MYSQL_HOST "tcp://127.0.0.1:3306"
#define MYSQL_USER "root"
#define MYSQL_PASS "root"
#define MYSQL_DB   "test"
#define MYSQL_TAB  "records"

// При использовании 1го коннектора возникают проблемы с выделением памяти
// Вполне возможно что он не поддерживает создание одновременно нескольких запросов
#define SINCLE_CONN 0

struct ThreadInfo {
    DatabaseCache *cache;
    DatabaseInterlayer *db;
    pthread_t id;
    intptr_t param;
};

void* syncTimer(void *attr);
bool thread_sync_timer(void *arg);
void* thread_function(void *arg);

int rand(int low, int high) {
   return rand() % (high - low + 1) + low;
}

void* syncTimer(void *attr) {
    for (;;) {
        sleep(DB_SYNC_EVERY);
        thread_sync_timer(attr);
    }
}

/**
 * Используем отдельные коннекты, поэтому синхронизацию вынесли отдельно
 */
bool thread_sync_timer(void *arg) {
    // Здесь нужно поставить проверку синхронизируется уже или нет
    std::cout << "synchronize" << std::endl;
    ThreadInfo *info = (ThreadInfo*)arg;
    DatabaseInterlayer *db = info->db;
    
    if (!db->isConnected()) {
        std::cout << "No connection" << std::endl;
        return false;
    }
    
    DatabaseCache::cache_map *cache = info->cache->getMap();
    if (cache->size()>0) {
        DatabaseCache::cache_iterator it = cache->begin();
        for (;it!=cache->end();it++) {
            if (it->second.isModified() && it->second.lock()) {
                if (!db->putToDB(it->first.c_str(), it->second.getData())) {
                    return false;
                }
                it->second.unlock();
            }
        }
    }
    return true;
}

/**
 * Генерирует случайные ключ/значения
 * Ключь от 1 до 100 чтобы потоки пытались одновременно обратица к записи
 */
void* thread_function(void *arg) {
    ThreadInfo *info = (ThreadInfo*)arg;
    std::string key, value;
    Record *r;
    DatabaseInterlayer *db = info->db;

    for (;;) {
        // Ключи от 1 до 100
        key = SSTR(rand(1, 100)); // std::to_string(rand(0, 100));
        // Случайное значение
        value = SSTR(rand()); // std::to_string(rand());
        
        if (db->getRecord(key.c_str(), r)) {
            try {
                if (r->lock()) {
                    std::cout << info->param << "] setRecordValue: " << key << " => " << value << " / " << r->getIteration() << std::endl;
                    r->setData(value.c_str());
                    r->unlock();
                } else {
                    std::cout << info->param << "] lockError: " << key << std::endl;
                }
            } catch (RecordExceptionTimeLock e) {
                std::cout << info->param << "] lockTimeout: " << key << std::endl;
            }
        } else {
            std::cout << info->param << "] getRecord: " << key << " => Error" << std::endl;
        }
        usleep(rand(100000, 500000));
    }
    
    return NULL;
}

int main(int argc, const char * argv[])
{
    std::cout << "Thread DB cache test!\n";
    
    // Random init
    srand((unsigned int)time(NULL));

    ThreadInfo threads[THREADS_COUNT+1];
    MemoryDatabaseCache *cache = new MemoryDatabaseCache();
#if SINCLE_CONN
    DatabaseInterlayer *db = new DatabaseInterlayer(MYSQL_TAB, cache);
    db->connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_DB);
#endif
    
    for (intptr_t i=0;i<THREADS_COUNT;i++) {
        threads[i].param = i;
        threads[i].cache = cache;
#if !SINCLE_CONN
        threads[i].db = new DatabaseInterlayer(MYSQL_TAB, cache);
        threads[i].db->connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_DB);
#else
        threads[i].db = db;
#endif
        pthread_create(&threads[i].id , NULL, thread_function, (void *)&threads[i]);
    }
    
    // Sturt sync timer
    threads[THREADS_COUNT].param = 0;
    threads[THREADS_COUNT].cache = cache;
#if !SINCLE_CONN
    threads[THREADS_COUNT].db = new DatabaseInterlayer(MYSQL_TAB, cache);
    threads[THREADS_COUNT].db->connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_DB);
#else
    threads[THREADS_COUNT].db = db;
#endif
    pthread_create(&threads[THREADS_COUNT].id, NULL, syncTimer, (void *)&threads[THREADS_COUNT]);

//    char c;
//    for (;;) {
//        sleep(3);
//        std::cin >> c;
//        if ('x'==c || 'X'==c) {
//            break;
//        } else {
//            std::cout << "If you want to go out PRINT X" << std::endl;
//        }
//    }

    sleep(60);
    
    // Close all threads
    for (int i=0;i<THREADS_COUNT;i++) {
        pthread_cancel(threads[i].id);
#if !SINCLE_CONN
        threads[i].db->disconnect();
        delete threads[i].db;
#endif
    }

#if SINCLE_CONN
    db->disconnect();
    delete db;
#endif

    cache->clear();
    delete cache;

    std::cout << "Complete!\n";
    
    return 0;
}

