#ifndef THREAD_H
#define THREAD_H


#include <iostream>
#include <sstream>
#include <string>

#include "FTPHandler.h"
#include "ReQueue.h"
#include "locks.h"

class Thread
{
    public:
        Thread();
        ~Thread();

        void Start();
//        bool IsRunning();
        void Run();
        void Wait();
        void Sleep(long mSeconds);
//        void Notify();
        void Join();

    private:
        ReQueue* que;
        std::thread *th;
        bool needWorking;
};

#endif // THREAD_H
