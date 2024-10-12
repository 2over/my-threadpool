//
// Created by xiehao on 2024/10/12.
//

#include "Threadpool.h"

#include "Threadpool.h"
#include "Thread.h"
#include "Taskpool.h"
#include "Task.h"

extern Taskpool taskpool;
extern Threadpool threadpool;

void* execute_task(void* args) {
    Thread* Self = static_cast<Thread *>(args);

    INFO_PRINT("[%s] 开始执行任务\n", Self->name().c_str());

    for(;;) {
        /**
         * 取人物执行
         * 加锁，同一时刻只有一个线程可以读写任务池
         */
        pthread_mutex_lock(taskpool._lock);

        while (0 == taskpool.task_count()) {
            INFO_PRINT("[%s] 暂无任务执行, 进入阻塞\n", Self->name().c_str());

            pthread_cond_wait(taskpool._cond, taskpool._lock);
            /**
             * 如果任务池中没有任务却被唤醒，表示是在销毁线程
             * 此段代码一定要放在wait下面，否则会导致某些线程执行完任务就退出了
             * 虽然也可以，但是不符合线程池理论
             */
            INFO_PRINT("[%s] live=%d, core=%d\n", Self->name().c_str(), threadpool.live_thread_size(), threadpool.core_pool_size());
        }
    }
}

