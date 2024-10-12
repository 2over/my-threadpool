//
// Created by xiehao on 2024/10/11.
//

#ifndef MY_THREADPOOL_PARKEVENT_H
#define MY_THREADPOOL_PARKEVENT_H

#include "../include/common.h"

class Thread;

class ParkEvent {
private:
    /**
     * 锁状态：
     *      0 未锁
     *      1 已锁
     */
    int _state;

    Thread *_owner;
    pthread_mutex_t _lock[1];
    pthread_cond_t _cond[1];
public:
    ParkEvent() {
        int status;

        _state = 0;

        status = pthread_mutex_init(_lock, NULL);

        assert(status == 0, "mutex init fail");

        status = pthread_cond_init(_cond, NULL);
        assert(status == 0, "cond init fail");
    }

    ParkEvent(Thread *owner) {
        int status;
        _owner = owner;
        _state = 0;

        status = pthread_mutex_init(_lock, NULL);
        assert(status == 0, "mutex init fail");

        status = pthread_cond_init(_cond, NULL);
        assert(status == 0, "cond init fail");
    }

public:
    void park();

    void unpark();

    int state() {
        return _state;
    }

    void lock();

    void unlock();

};


#endif //MY_THREADPOOL_PARKEVENT_H
