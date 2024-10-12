//
// Created by xiehao on 2024/10/11.
//

#ifndef MY_THREADPOOL_TASKPOOL_H
#define MY_THREADPOOL_TASKPOOL_H

#include "../include/common.h"

class Task;

class Taskpool {
private:
    int _task_count;
    Task *_tasks;
public:
    pthread_mutex_t _lock[1];
    pthread_cond_t _cond[1];

    Taskpool() {
        _tasks = 0;

        int status = pthread_mutex_init(_lock, NULL);
        assert(status == 0, "mutex init fail");

        status = pthread_cond_init(_cond, NULL);
        assert(status == 0, "cond init fail");
    }

public:
    int task_count() {
        return _task_count;
    }

    void AddTask(Task *node);

    Task *DequeueTask();

    void DequeueSpecificTask(Task *node);

    void desc_task_count() {
        _task_count--;
    }

    void inc_task_count(int step) {
        _task_count += step;
    }
};


#endif //MY_THREADPOOL_TASKPOOL_H
