//
// Created by xiehao on 2024/10/12.
//

#ifndef MY_THREADPOOL_THREADPOOL_H
#define MY_THREADPOOL_THREADPOOL_H

#include "../include/common.h"

enum ThreadpoolState {
    NEW,        // 线程池正在创建
    RUNNING,    // 线程池已创建完成，可以接受人物
    STOP,       // 暂停接人物
    CLOSING,    // 线程池正在关闭
    CLOSED,     // 关闭
};

class Thread;


class Threadpool {
private:
    pthread_t _manager_thread_id[1];
    Thread **_thread_ids;


    ThreadpoolState _state;
public:
    pthread_mutex_t _lock[1];
    pthread_cond_t _cond[1];

private:
    int _core_pool_size;
    int _max_pool_size;

private:
    int _busy_thread_size;  // 繁忙的线程数
    int _idle_thread_size;  // 空闲的线程数
    int _live_thread_size;  // 存活线程数

public:
    Threadpool(int core_pool_size, int max_pool_size);

    ~Threadpool() {
        free(_thread_ids);
        _thread_ids = NULL;
    }

public:
    int live_thread_size() {
        return _live_thread_size;
    }

    Threadpool *inc_live_thread_size() {
        _live_thread_size++;
        return this;
    }

    Threadpool *desc_live_thread_size() {
        _live_thread_size--;
        return this;
    }

    int busy_thread_size() {
        return _busy_thread_size;
    }

    Threadpool *inc_busy_thread_size() {
        _busy_thread_size++;
        return this;
    }

    Threadpool *desc_busy_thread_size() {
        _busy_thread_size--;
        return this;
    }

    int idle_thread_size() {
        return _idle_thread_size;
    }

    Threadpool *inc_idle_thread_size() {
        _idle_thread_size++;
        return this;
    }

    Threadpool *desc_idle_thread_size() {
        _idle_thread_size--;
        return this;
    }

    int core_pool_size() {
        return _core_pool_size;
    }

    int max_pool_size() {
        return _max_pool_size;
    }

    Thread **thread_ids() {
        return _thread_ids;
    }

    void run();

    void expand();

    void shrink();

};


#endif //MY_THREADPOOL_THREADPOOL_H
