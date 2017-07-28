#include "ThreadPool.h"

ThreadPool::ThreadPool(int size)
{
    //ctor
    this->size = size;
    this->threads = new Thread[size];
    requeue = ReQueue::getInstance();
}

ThreadPool::~ThreadPool()
{
    //dtor
    delete[] threads;
}

void ThreadPool::startAll(){
    for(int i = 0; i < size; i++){
        threads[i].Start();
    }
}

void ThreadPool::push(Socket *socket){
    requeue->enQueue(socket);
}

void ThreadPool::joinAll(){
    for(int i = 0; i < size; i++){
        threads[i].Join();
    }
}
