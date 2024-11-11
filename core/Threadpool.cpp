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

void *execute_task(void *args) {
    Thread *Self = static_cast<Thread *>(args);

    INFO_PRINT("[%s] 开始执行任务\n", Self->name().c_str());

    for (;;) {
        /**
         * 取任务执行
         * 加锁，同一时刻只有一个线程可以读写任务池
         */
        pthread_mutex_lock(taskpool._lock);

        while (0 == taskpool.task_count()) {
            INFO_PRINT("[%s] 暂无任务执行, 进入阻塞\n", Self->name().c_str());

            // wait做了什么
            // 1.执行了unlock
            // 2.线程进入阻塞，阻塞在条件变量上
            // 3.唤醒之后又会进行lock
            pthread_cond_wait(taskpool._cond, taskpool._lock);
            /**
             * 如果任务池中没有任务却被唤醒，表示是在销毁线程
             * 此段代码一定要放在wait下面，否则会导致某些线程执行完任务就退出了
             * 虽然也可以，但是不符合线程池理论
             */
            INFO_PRINT("[%s] live=%d, core=%d\n", Self->name().c_str(), threadpool.live_thread_size(),
                       threadpool.core_pool_size());
            if (threadpool.live_thread_size() > threadpool.core_pool_size()) {
                INFO_PRINT("[%s] 销毁\n", Self->name().c_str());

                threadpool.desc_live_thread_size();

                // 这里一定要记得解锁，否则二次销毁的时候，wait内部是上锁的
                pthread_mutex_unlock(taskpool._lock);

                pthread_exit(NULL);

            }
        }

        // 获取任务、解锁，执行任务
        INFO_PRINT("[%s] 任务池中有任务了，开始抢任务\n", Self->name().c_str());

        Task *task = taskpool.DequeueTask();

        taskpool.desc_task_count();

        INFO_PRINT("[%s] 抢到了任务 %d, 剩余任务数: %d\n", Self->name().c_str(), task->num(), taskpool.task_count());

        pthread_mutex_unlock(taskpool._lock);

        // 处理线程池数据
        pthread_mutex_lock(threadpool._lock);
        threadpool.inc_busy_thread_size();
        threadpool.desc_idle_thread_size();
        pthread_mutex_unlock(threadpool._lock);

        INFO_PRINT("[%s] 繁忙线程数量 %d\n", Self->name().c_str(), threadpool.busy_thread_size());

        /**
         * 模拟执行任务花费的时间
         * 这个执行任务可以在解锁之后执行，然后再通过加锁修改繁忙与空闲线程数量
         * 如果执行的任务比较耗时，锁一直无法释放，其他任务就无法被执行
         * 修改繁忙与空闲线程数量只是影响线程的扩容与所容，实时性要求没那么高
         */
        sleep(5);

        pthread_mutex_lock(threadpool._lock);
        threadpool.desc_busy_thread_size();
        threadpool.inc_idle_thread_size();
        pthread_mutex_unlock(threadpool._lock);

        INFO_PRINT("[%s] 任务执行结束\n", Self->name().c_str());

    }
    return 0;
}

void Threadpool::run() {
    for (int i = 0; i < _core_pool_size; ++i) {
        Thread *t = new Thread(execute_task, NULL, i + 1);

        thread_ids()[i] = t;

        t->run();
    }
}

Threadpool::Threadpool(int core_pool_size, int max_pool_size) {
    _core_pool_size = core_pool_size;
    _max_pool_size = max_pool_size;

    _busy_thread_size = 0;
    _idle_thread_size = _core_pool_size;
    _live_thread_size = _core_pool_size;

    _state = NEW;

    _thread_ids = static_cast<Thread **>(calloc(_max_pool_size, sizeof(Thread *)));

    pthread_mutex_init(_lock, NULL);
    pthread_cond_init(_cond, NULL);
}

void Threadpool::expand() {
    for (int i = _live_thread_size; i < _max_pool_size; i++) {
        Thread* t = new Thread(execute_task, NULL, i + 1);
        thread_ids()[i] = t;

        t->run();

        inc_live_thread_size();
    }
}

void Threadpool::shrink() {
    pthread_cond_signal(taskpool._cond);
};

