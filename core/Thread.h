//
// Created by xiehao on 2024/10/11.
//

#ifndef MY_THREADPOOL_THREAD_H
#define MY_THREADPOOL_THREAD_H

#include "../include/common.h"

class Monitor;

class ParkEvent;

enum ThreadState {
    ALLOCATED,              // Memory has been allocated but not initialized
    INITIALIZED,            // The thread has been initialized but yet started
    RUNNABLE,               // Has been started and is runnable ,but not necessarily running
    MONITOR_WAIT,           // Waiting on a contended monitor lock
    CONDVAR_WAIT,           // Waiting on a condition variable
    OBJECT_WAIT,            // Waiting on an Object.wait() call
    BREAKPOINTED,           // Suspended at breakpoint
    SLEEPING,               // Thread.sleep()
    ZOMBIE                  // All done, but not reclaimed yet

};

class Thread {

private:
    string _name;

    ThreadState _state;
    bool _interrupted;

private:
    pthread_t _tid[1];

    thread_fun _entry_point;

    void *_args;

public:
    void set_interrupted(bool v) {
        _interrupted = v;
    }

    bool interrupted() {
        return _interrupted;
    }

    void set_state(ThreadState state) {
        _state = state;
    }

    ThreadState get_state() {
        return _state;
    }

    Thread(thread_fun entry_point, void *args, char *name);

    Thread(thread_fun entry_point, void *args, int thread_index);

    ~Thread();

public:
    ParkEvent *_ParkEvent;         // for synchronized()
    ParkEvent *_MutexEvent;        // for native internal Mutex/Monitor

private:
    Monitor *_startThread_lock;      // sync parent and child in thread creation

public:
    Monitor *startThread_lock() {
        return _startThread_lock;
    }

public:
    pthread_t tid() {
        return *_tid;
    }

    thread_fun entry_point() {
        return _entry_point;
    }

    string name() {
        return _name;
    }

public:
    void run();
};


#endif //MY_THREADPOOL_THREAD_H
