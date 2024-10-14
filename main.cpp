#include <iostream>
#include "core/Threadpool.h"
#include "core/Taskpool.h"
#include "core/Task.h"
#include "core/ManagerThread.h"
#include "core/ParkEvent.h"

Taskpool taskpool;

Threadpool threadpool(3, 5);

int main() {

    threadpool.run();

    sleep(2);

    ManagerThread::run();

    sleep(1);
    int v;
    while (true) {
        printf("[主线程] 请输入插入任务的条数: \n");

        cin >> v;
        for (int i = 0; i < v; ++i) {
            taskpool.AddTask(new Task(i + 1));
        }

        taskpool.inc_task_count(v);

        if (ManagerThread::_locked) {
            ManagerThread::_Event-> unlock();

            ManagerThread::_locked = false;
        }

        pthread_cond_broadcast(taskpool._cond);
        INFO_PRINT("[主线程]唤醒管理者线程、线程池开始工作\n");
    }
    return 0;
}
