//
//  Record.cpp
//  ThreadDBTest
//
//  Created by Dmitry Ponomarev on 6/6/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

/**
 * @INFO: Mac OS X unsupport POSIX pthread_mutex_timedlock
 */

#include <unistd.h>
#include <iostream>

#include "Record.h"

bool Record::lock(int waitSeconds) {
    if (waitSeconds>0) {
        // Пытаемся заблокировать поток
        if (0!=pthread_mutex_trylock(&_mutex)) {
            // Если не получается засыпаем на указанное время
            std::cout << "Resource locked. Wait " << waitSeconds << " second(s)" << std::endl;
            sleep(waitSeconds);
            // Снова пытаемся заблокировать поток
            if (0!=pthread_mutex_trylock(&_mutex)) {
                // Не вышло :( Кидаем исключение
                throw RecordExceptionTimeLock(waitSeconds);
            }
        }
    } else {
        return 0==pthread_mutex_lock(&_mutex);
    }
    return true;
}

void Record::unlock(void) {
    _modified = true;
    _iteration++;
    pthread_mutex_unlock(&_mutex);
}


void Record::setData(const char* data) {
    _data = data;
}

const char* Record::getData(void) const {
    return _data.c_str();
}

int Record::getIteration(void) const {
    return _iteration;
}

void Record::clear(void) {
    _data.clear();
}

bool Record::isModified(void) const {
    return _modified;
}

