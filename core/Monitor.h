//
// Created by xiehao on 2024/10/11.
//

#ifndef MY_THREADPOOL_MONITOR_H
#define MY_THREADPOOL_MONITOR_H

class Thread;

class Monitor {
private:
    Thread *_owner;
public:
    void wait(Thread *t);

    void notify(Thread *t);

    void notify_all(Thread *t);
};


#endif //MY_THREADPOOL_MONITOR_H
