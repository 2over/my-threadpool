//
// Created by xiehao on 2024/10/11.
//

#ifndef MY_THREADPOOL_TASK_H
#define MY_THREADPOOL_TASK_H

#include "../include/common.h"

class Task {
private:
    // 任务编号
    int _num;

public:
    Task *_prev;
    Task *_next;

public:
    Task(int num) {
        _num = num;

        _prev = NULL;
        _next = NULL;
    }

    int num() {
        return _num;
    }
};


#endif //MY_THREADPOOL_TASK_H
