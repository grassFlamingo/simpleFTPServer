#include "Thread.h"

#include "locks.h"

Thread::Thread()
{
    //ctor
    needWorking = true;
    th = NULL;
    que = ReQueue::getInstance();
}

Thread::~Thread()
{
    //dtor
}

void Thread::Start(){
    if(th){
        return;
    }
    gVar.notify_one();
    th = new std::thread(&Thread::Run, this);
    th->detach();
}

void Thread::Wait(){
    std::unique_lock<std::mutex> lc(gLock);
    gVar.wait(lc);
}

void Thread::Run(){
    Socket* soc = NULL;
    FTPHandler ftp;
    while(needWorking){
        try{
            soc = que->deQueue();
            ftp.connectSucceed(soc);
            printf("Connected: %s -> %s\n", soc->ToStringMyIPPort(),
                   soc->ToStringClientIPPort());
            ftp.exec(soc);
        }catch(Exception *e){
            printf("%s\n", e->getMessage());
            e->handeled();
            delete e;
        }
        printf("Disconnected: %s -> %s\n", soc->ToStringMyIPPort(),
               soc->ToStringClientIPPort());
        delete soc;
    }
}

//void Thread::Notify(){
//    gVar.notify_one();
//}

void Thread::Sleep(long mSeconds){
    std::chrono::milliseconds dura(mSeconds);
    std::this_thread::sleep_for(dura);
}

void Thread::Join(){
    needWorking = false;
    th->join();
}
