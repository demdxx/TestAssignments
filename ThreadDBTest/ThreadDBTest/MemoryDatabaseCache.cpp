//
//  MemoryDatabaseCache.cpp
//  ThreadDBTest
//
//  Created by Dmitry Ponomarev on 6/7/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#include "MemoryDatabaseCache.h"

/**
 * @param key
 * @param value
 * @return bool
 */
bool MemoryDatabaseCache::getRecord(const char *key, Record *&record) {
    cache_iterator it = _cache.find(key);
    if (it!=_cache.end()) {
        record = &it->second;
        return true;
    }
    return false;
}

/**
 * @param key
 * @param value
 * @return bool
 */
bool MemoryDatabaseCache::setRecord(const char *key, const char *value) {
    cache_iterator it = _cache.find(key);
    const char *v = NULL!=value ? value : "";
    if (it!=_cache.end()) {
        it->second.setData(v);
    } else {
        _cache.insert(std::pair<std::string, Record>(key, Record(v)));
    }
    return true;
}

/**
 * return MemoryDatabaseCache::cache_map*
 */
MemoryDatabaseCache::cache_map* MemoryDatabaseCache::getMap(void) {
    return &_cache;
}

void MemoryDatabaseCache::clear(void) {
    _cache.clear();
}
