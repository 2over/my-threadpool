//
// Created by xiehao on 2024/10/11.
//

#include "Monitor.h"
#include "Thread.h"
#include "ParkEvent.h"

void Monitor::wait(Thread *t) {
    ParkEvent *event = t->_MutexEvent;
    assert(NULL != event, "请创建_MutexEvent");

    event->park();
}

void Monitor::notify(Thread *t) {
    ParkEvent *event = t->_MutexEvent;
    assert(NULL != event, "请创建_MutexEvent");

    event->unpark();
}

