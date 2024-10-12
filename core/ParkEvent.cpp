//
// Created by xiehao on 2024/10/11.
//

#include "ParkEvent.h"
#include "Thread.h"

void ParkEvent::park() {
    // 这里为什么要加锁? 因为条件变量wiat必须要先加锁
    pthread_mutex_lock(_lock);

    _state = 1;

    INFO_PRINT("[%s] 完成初始化\n", _owner->name().c_str());

    _owner->set_state(INITIALIZED);

    pthread_cond_wait(_cond, _lock);

    pthread_mutex_unlock(_lock);
}

void ParkEvent::unpark() {
    pthread_mutex_lock(_lock);

    _state = 0;

    pthread_cond_signal(_cond);

    pthread_mutex_unlock(_lock);
}

void ParkEvent::lock() {
    pthread_mutex_lock(_lock);

    _state = 1;

    pthread_cond_wait(_cond, _lock);

    pthread_mutex_unlock(_lock);
}

void ParkEvent::unlock() {
    pthread_mutex_lock(_lock);

    _state = 0;

    pthread_cond_signal(_cond);

    pthread_mutex_unlock(_lock);
}