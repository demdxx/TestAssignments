//
//  MemoryDatabaseCache.h
//  ThreadDBTest
//
//  Created by Dmitry Ponomarev on 6/7/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#ifndef __ThreadDBTest__MemoryDatabaseCache__
#define __ThreadDBTest__MemoryDatabaseCache__

#include <string>
//#include <unordered_map> // awailable in clib llvm
#include <boost/unordered_map.hpp>

#include "DatabaseCache.h"

class MemoryDatabaseCache : public DatabaseCache {
private:
    typedef boost::unordered_map<std::string, Record> cache_map;
    typedef cache_map::iterator cache_iterator;

    cache_map _cache;

public:
    virtual bool getRecord(const char *key, Record *&record);
    virtual bool setRecord(const char *key, const char *value);
    virtual cache_map* getMap(void);
    // Не использовать при обработке записей
    virtual void clear(void);
};

#endif /* defined(__ThreadDBTest__MemoryDatabaseCache__) */
