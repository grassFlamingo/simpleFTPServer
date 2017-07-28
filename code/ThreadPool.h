#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "Thread.h"

class ThreadPool
{
    public:
        ThreadPool(int size);
        ~ThreadPool();

        void push(Socket* socket);
        void startAll();
        void joinAll();

    protected:

    private:
        int size;
        ReQueue* requeue;
        Thread* threads;
};

#endif // THREADPOOL_H
