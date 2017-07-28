#ifndef SOCKET_H
#define SOCKET_H

#include <winsock2.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "exception.h"
#include "Rwexception.h"
#include "mystring.h"

#define BUFFER_SIZE 2048
typedef unsigned short UShort;


typedef union{
    u_char cport[2];
    unsigned short iport;
}Port;

typedef union{
    u_char hotes[4];
    u_long L_addr;
}IPAddress;

struct IPPort{
    IPAddress ip;
    Port port;
};

class Socket{
public:
    Socket(int family, int type, int protocol);
    ~Socket();

    static void init();
    static void exit();

    void Bind(UShort port);
    void Close();
    void Listen(int backlog);

    void Bind(IPAddress &ip, UINT port);

    void Send(const Bytes *byte);
    void Send(const Bytes *byte, int size);
    bool Recv(int size = 1024);
    bool Recv(int *size);
    Bytes* GetRecvData();

    Socket* Accept();
    void Connect(IPAddress &ip, UINT port);

    IPPort* GetMyIPPort();
    IPPort* GetClientIPPort();
    Bytes* ToStringMyIPPort();
    Bytes* ToStringClientIPPort();

private:
    Socket(int id);
    void setIPPort();

    IPPort myIPort;
    IPPort hisIPort;

    SOCKET socketID;
    Bytes recvBuffer[BUFFER_SIZE];
    Bytes iPort[30];
    Bytes hPort[30];
};

//    int i;
//    char szHost[256];
//    gethostname(szHost,256);
//    hostent *pHost=gethostbyname(szHost);
//    in_addr addr;

//    for(int i = 0; i < pHost->h_length-1; i++){
//        char *p=pHost->h_addr_list[i];
//        memcpy(&addr.S_un.S_addr,p,pHost->h_length);
//        char *szIp=inet_ntoa(addr);
//        printf("szIp %s\n",szIp);
//    }

#endif // SOCKET_H
