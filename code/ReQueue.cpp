#include "ReQueue.h"

#include "locks.h"

QueueMember::QueueMember(Socket* socket){
    this->socket = socket;
    this->next = NULL;
}

QueueMember::~QueueMember(){
}

void QueueMember::setNext(QueueMember* next){
    this->next = next;
}

void QueueMember::removeOneNext(){
    QueueMember* tnex = next;
    if(next == NULL){
        return;
    }
    next = next->getNext();
    delete tnex;
}

void QueueMember::removeAllNext(){
    if(next){
        removeAllNext();
        delete next;
    }
}

Socket* QueueMember::getSocket(){
    return socket;
}

QueueMember* QueueMember::getNext(){
    return next;
}

bool QueueMember::hasNext(){
    return next != NULL;
}


ReQueue* ReQueue::me = new ReQueue();

ReQueue::ReQueue()
{
    //ctor
    root = new QueueMember();
    head = root;
    tail = root;
}

ReQueue::~ReQueue()
{
    delete root;
    //dtor
}

ReQueue* ReQueue::getInstance(){
    return me;
}

void ReQueue::enQueue(Socket *soc){
    std::mutex mutex;
    mutex.lock();
    if(soc == NULL){
        return;
    }
    QueueMember* member = new QueueMember(soc);
    tail->setNext(member);
    tail = tail->getNext();
    gVar.notify_one();
    mutex.unlock();
}

Socket* ReQueue::deQueue(){
    std::mutex mutex;
    mutex.lock();
    while(!head->hasNext()){
        std::unique_lock<std::mutex> lc(gLock);
        gVar.wait(lc);
    }
    QueueMember* hNext = head->getNext();
    Socket* nextSock = hNext->getSocket();
    if(hNext == tail){
        tail = head;
    }
    head->removeOneNext();
    mutex.unlock();
    return nextSock;
}
