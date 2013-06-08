//
//  DatabaseCache.h
//  ThreadDBTest
//
//  Created by Dmitry Ponomarev on 6/7/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#ifndef __ThreadDBTest__DatabaseCache__
#define __ThreadDBTest__DatabaseCache__

#include "Record.h"

class DatabaseCache {
public:
    typedef boost::unordered_map<std::string, Record> cache_map;
    typedef cache_map::iterator cache_iterator;

    virtual bool getRecord(const char *key, Record *&record) = 0;
    virtual bool setRecord(const char *key, const char *value) = 0;
    virtual cache_map* getMap(void) = 0;
    virtual void clear(void) = 0;
};

#endif /* defined(__ThreadDBTest__DatabaseCache__) */
