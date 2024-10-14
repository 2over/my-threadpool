//
// Created by ziya on 2024/10/14.
//

#include "ManagerThread.h"
#include "ManagerThread.h"
#include "Threadpool.h"
#include "Taskpool.h"
#include "ParkEvent.h"

extern Taskpool taskpool;
extern Threadpool threadpool;

bool ManagerThread::_locked = false;
ParkEvent* ManagerThread::_Event = new ParkEvent;

void* thread_manager_do(void* args) {
    INFO_PRINT("管理者线程开始启动\n");

    int min_thread_size = 0;
    int busy_thread_size = 0;
    int live_thread_size = 0;
    int max_pool_size = 0;
    int task_count = 0;

    for (;;) {
        min_thread_size =  threadpool.core_pool_size();
        busy_thread_size = threadpool.busy_thread_size();
        live_thread_size = threadpool.live_thread_size();
        max_pool_size = threadpool.max_pool_size();

        INFO_PRINT("busy=%d, live=%d, max=%d, task_count=%d\n", busy_thread_size, live_thread_size, max_pool_size, task_count);

        /**
         * 扩容需要满足的条件:
         * 1.所有线程都在忙碌
         * 2.当前线程数少于最大线程数(两个思路:一次到最大、慢慢扩容)
         * 3.还有未执行的任务
         */
        if (busy_thread_size == live_thread_size && busy_thread_size < max_pool_size  && task_count > 0) {
            INFO_PRINT("[管理者线程] 线程池需要扩容\n");

            threadpool.expand();
        }

        /**
         * 所容需要满足的条件:
         * 1.当期线程数超过核心线程数
         * 2.没有任务需要执行
         * 3.没有线程在执行任务
         */
        if (0 == task_count && 0 == busy_thread_size && live_thread_size > min_thread_size) {
            INFO_PRINT("[管理者线程] 线程池需要缩容\n");

            threadpool.shrink();
        }

        // 自己进入休眠
        if (0 == task_count && live_thread_size == min_thread_size) {
            INFO_PRINT("[管理者线程] 进入休眠,等待唤醒\n");
            ManagerThread::_locked = true;
            ManagerThread::_Event->lock();
        }

        sleep(2);
    }
}

void ManagerThread::run() {
    pthread_t tid;
    pthread_create(&tid, NULL, thread_manager_do, NULL);
}

