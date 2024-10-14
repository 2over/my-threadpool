//
// Created by ziya on 2024/10/14.
//

#ifndef MY_THREADPOOL_MANAGERTHREAD_H
#define MY_THREADPOOL_MANAGERTHREAD_H

class ParkEvent;

class ManagerThread {
public:
    static bool _locked;
    static ParkEvent* _Event;

    static void run();
};


#endif //MY_THREADPOOL_MANAGERTHREAD_H
