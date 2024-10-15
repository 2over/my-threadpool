//
// Created by xiehao on 2024/10/11.
//

#include "Thread.h"
#include "Monitor.h"
#include "ParkEvent.h"

void *thread_run(void *arg) {
    Thread *t = static_cast<Thread *>(arg);

    INFO_PRINT("[%s] 等待创建线程工作完成\n", t->name().c_str());

    Monitor *sync = t->startThread_lock();

    sync->wait(t);

    INFO_PRINT("[%s] 创建工作已完成，线程被唤醒\n", t->name().c_str());

    // 执行业务逻辑
    t->entry_point()(t);

    return 0;
}

Thread::Thread(thread_fun entry_point, void *args, char *name) {
    _entry_point = entry_point;
    _args = args;
    _name = name;

    set_state(ALLOCATED);

    int status = pthread_create(_tid, NULL, thread_run, this);

    ASSERT_ZERO(status, "crete thread fail\n");

    _startThread_lock = new Monitor;

    _MutexEvent = new ParkEvent(this);
    _ParkEvent = new ParkEvent(this);
}

Thread::Thread(thread_fun entry_point, void *args, int thread_num) {
    _entry_point = entry_point;

    _args = args;

    stringstream ss;
    ss << "t";
    ss << thread_num;

    _name = ss.str();

    set_state(ALLOCATED);

    _MutexEvent = new ParkEvent(this);
    _ParkEvent = new ParkEvent(this);

    int status = pthread_create(_tid, NULL, thread_run, this);
    ASSERT_ZERO(status, "create thread fail\n");
}

/**
 * 会出现的情况：
 * 1.run比线程函数先执行
 * 解决办法: 借助线程状态
 */
void Thread::run() {
    assert(NULL != _entry_point, "entry point is null");

    while (true) {
        if (INITIALIZED == get_state()) {
            INFO_PRINT("[notify] 创建线程工作已完成， 尝试唤醒线程\n");

            set_state(RUNNABLE);

            startThread_lock()->notify(this);

            break;
        } else {
//            INFO_PRINT("[%s] 等待线程完成初始化 %d \n", name().c_str(), get_state());
        }
    }

}