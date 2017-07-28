#ifndef REQUEUE_H
#define REQUEUE_H


#include "Socket.h"

class QueueMember{
public:
    QueueMember(Socket* socket = NULL);
    ~QueueMember();

    void setNext(QueueMember* next);
    void removeOneNext();
    void removeAllNext();

    bool hasNext();

    Socket* getSocket();
    QueueMember* getNext();

private:
    Socket* socket;
    QueueMember* next;
};

class ReQueue
{
    public:
        static ReQueue* getInstance();

        void enQueue(Socket* soc);
        Socket* deQueue();

    private:
        ReQueue();
        ~ReQueue();
        static ReQueue* me;
        QueueMember* root;
        QueueMember* head;
        QueueMember* tail;
};

#endif // REQUEUE_H
