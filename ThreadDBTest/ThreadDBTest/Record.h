//
//  Record.h
//  ThreadDBTest
//
//  Created by Dmitry Ponomarev on 6/6/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#ifndef __ThreadDBTest__Record__
#define __ThreadDBTest__Record__

#define LOCK_RECORD_TIME 2 // Seconds

#include <iostream>
#include <pthread.h>
//#include <boost/thread/mutex.hpp>

class RecordExceptionTimeLock {
private:
    int _lockTime;

public:
    inline RecordExceptionTimeLock(int time) : _lockTime(time) {}

    inline int lockTime(void) {
        return LOCK_RECORD_TIME;
    }
};

class Record {
private:
    std::string _data;
    pthread_mutex_t _mutex;
    bool _modified;
    int _iteration;
//    boost::timed_mutex _mutex;

public:
    inline Record(void)
//            : _mutex(PTHREAD_MUTEX_INITIALIZER)
            : _modified(false)
            , _iteration(0) {
//        _mutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_init(&_mutex, NULL);
    }
    inline Record(const char* data)
//            : _mutex(PTHREAD_MUTEX_INITIALIZER)
            : _modified(false)
            , _data(data)
            , _iteration(0) {
//        _mutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_init(&_mutex, NULL);
    }
    
    virtual ~Record(void) {
        unlock();
        pthread_mutex_destroy(&_mutex);
    }

    void setData(const char* data);
    const char* getData(void) const;
    int getIteration(void) const;
    void clear(void);
    
    bool isModified(void) const;

    bool lock(int waitSeconds=LOCK_RECORD_TIME);
    void unlock(void);

// Operators
    inline void operator = (const char* data) {
        setData(data);
    }
    
    inline void operator = (Record& r) {
        _modified = r.isModified();
        _iteration = r.getIteration();
        setData(r.getData());
    }
    
    inline operator const char*(void) {
        return getData();
    }
};

#endif /* defined(__ThreadDBTest__Record__) */
