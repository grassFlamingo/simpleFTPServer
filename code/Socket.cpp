#include "Socket.h"


Socket::Socket(int family, int type, int protocol)
{
    // sockid: socket descriptor, an integer (like a file-handle)
    // family: integer, communication domain, e.g.,
    // PF_INET, IPv4 protocols, Internet addresses (typically used)
    // PF_UNIX, Local communication, File addresses
    // type: communication type
    // SOCK_STREAM - reliable, 2-way, connection-based service
    // SOCK_DGRAM - unreliable, connectionless, messages of maximum length
    // protocol: specifies protocol
    // IPPROTO_TCP IPPROTO_UDP
    // usually set to 0 (i.e., use default protocol)
    // upon failure returns -1
    // NOTE: socket call does not specify where data will be coming from,
    // nor where it will be going to it just creates the interface!

    socketID = socket(family, type, protocol);

    if(socketID == INVALID_SOCKET) {
        char err[100];
        sprintf(err,"ERROR at SOCKET() %d at file %s - Line %d",
                WSAGetLastError(), __FILE__, __LINE__);
        WSACleanup();
        throw new Exception(err,100);
    }
}

Socket::Socket(int id){
    socketID = id;
    setIPPort();
}


Socket::~Socket()
{
    try{
        Close();
    }catch(Exception* e){
        e->handeled();
        delete e;
    }
}


void Socket::init(){
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(iResult != NO_ERROR){
        char err[100];
        sprintf(err,"Socket Init Error. at file %s - Line %d",
                __FILE__, __LINE__);
        throw new Exception(err,100);
    }
}

void Socket::exit(){
    WSACleanup();
}

void Socket::Bind(UShort port){
    //    htonl()--"Host to Network Long"
    //    ntohl()--"Network to Host Long"
    //    htons()--"Host to Network Short"
    //    ntohs()--"Network to Host Short"
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    int bindid = bind(socketID, (sockaddr *)&addr, sizeof(addr));
    if(bindid < 0){
        char err[100];
        sprintf(err,"Socket: %d Bind: %d Error. at file %s - Line %d",
                socketID, port, __FILE__, __LINE__);
        throw new Exception(err,100);
    }
    setIPPort();
}

void Socket::Bind(IPAddress &ip, UINT port){

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = ip.L_addr;
    addr.sin_port = htons(port);

    int bindid = bind(socketID, (sockaddr *)&addr, sizeof(addr));
    if(bindid < 0){
        char err[100];
        sprintf(err,"Socket: %d Bind: %d Error %d. at file %s - Line %d",
                socketID, port,WSAGetLastError(), __FILE__, __LINE__);
        throw new Exception(err,100);
    }
    setIPPort();
}

void Socket::Listen(int backlog){
    int status = listen(socketID, backlog);
    //    status: 0 if listening, -1 if error
    if(status < 0){
        char err[100];
        sprintf(err,"Socket: %d Listen: %d Error. at file %s - Line %d",
                socketID, backlog, __FILE__, __LINE__ );
        throw new Exception(err,100);
    }
}

void Socket::Close(){
    //     status: 0 if successful, -1 if error
    int status = closesocket(socketID);
    if(status < 0){
        char err[100];
        sprintf(err,"Socket: %d Close Error. at file %s - Line %d",
                socketID, __FILE__ , __LINE__);
        throw new Exception(err,100);
    }
    memset(recvBuffer,0,BUFFER_SIZE);
}

void Socket::Send(const Bytes* byte){
    //    int count = send(sockid, msg, msgLen, flags);
    //    msg: const void[], message to be transmitted
    //    msgLen: integer, length of message (in bytes) to transmit
    //    flags: integer, special options, usually just 0
    //    count: # bytes transmitted (-1 if error)
    int count = send(socketID, byte, myString::byteLen(byte), 0);
    if(count < 0){
        char err[100];
        sprintf(err,"Socket: %d Send Error %d. at file %s - Line %d",
                socketID, WSAGetLastError() , __FILE__ , __LINE__);
        throw new RwException(err,100);
    }
}

void Socket::Send(const Bytes *byte, int size){
    int count = send(socketID, byte, size, 0);
    if(count < 0){
        char err[100];
        sprintf(err,"Socket: %d Send Error %d. at file %s - Line %d",
                socketID, WSAGetLastError() , __FILE__ , __LINE__);
        throw new RwException(err,100);
    }
}

bool Socket::Recv(int size){
    //    int count = recv(sockid, recvBuf, bufLen, flags);
    //    recvBuf: void[], stores received bytes
    //    bufLen: # bytes received
    //    flags: integer, special options, usually just 0
    //    count: # bytes received (-1 if error)
    if(size >= BUFFER_SIZE - 1){
        Bytes err[200];
        sprintf(err, "Recve Size %d is too large. at file: %s line %d",
                size, __FILE__, __LINE__ );
        throw new RwException(err,200);
    }
    int count = recv(socketID, recvBuffer, size, 0);
    if(count <= 0){
        return false;
    }
    recvBuffer[count] = '\0'; // add end
    return true;
}

bool Socket::Recv(int *size){
    if(*size >= BUFFER_SIZE - 1){
        Bytes err[200];
        sprintf(err, "Recve Size %d is too large. at file: %s line %d",
                *size, __FILE__, __LINE__ );
        throw new RwException(err,200);
    }
    int count = recv(socketID, recvBuffer, *size, 0);
    if(count <= 0){
        return false;
    }
    recvBuffer[count] = '\0'; // add end
    *size = count;
    return true;
}

Bytes* Socket::GetRecvData(){
    return recvBuffer;
}

Socket* Socket::Accept(){
    sockaddr_in address;
    int len,id;
    len = sizeof(address);
    id = accept(socketID, (sockaddr*)&address, &len);
    return new Socket(id);
}

void Socket::Connect(IPAddress &ip,UINT port){
    //    int status = connect(sockid, &foreignAddr, addrlen);
    //    sockid: integer, socket to be used in connection
    //    foreignAddr: struct sockaddr: address of the passive participant
    //    addrlen: integer, sizeof(name)
    //    status: 0 if successful connect, -1 otherwise
    //    connect() is blocking
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.S_un.S_addr = ip.L_addr;
    int status = connect(socketID,(sockaddr*)&server,sizeof(server));
    setIPPort();
    if(status < 0){
        char err[100];
        sprintf(err,"Socket: %s Connect to %s Error %d. at file %s - Line %d",
                ToStringMyIPPort(), ToStringClientIPPort(),
                WSAGetLastError(), __FILE__ , __LINE__);
        throw new Exception(err,100);
    }
}

/*
 * s.getpeername() # 返回连接套接字的远程地址。
 * s.getsockname() # 返回套接字自己的地址。
 */
void Socket::setIPPort(){
    sockaddr_in sin;
    int inlen = sizeof(sin);
    getsockname(socketID, (sockaddr *)&sin, &inlen);
    if(sin.sin_family == AF_INET){
        myIPort.port.iport = ntohs(sin.sin_port);
        myIPort.ip.L_addr = sin.sin_addr.S_un.S_addr;
    }

    sockaddr_in sout;
    int outLen = sizeof(sout);
    getpeername(socketID, (sockaddr *)&sout, &outLen);
    if(sout.sin_family == AF_INET){
        hisIPort.port.iport = ntohs(sout.sin_port);
        hisIPort.ip.L_addr = sout.sin_addr.S_un.S_addr;
    }
    sprintf(iPort,"(%d.%d.%d.%d:%d)",
            myIPort.ip.hotes[0], myIPort.ip.hotes[1],
            myIPort.ip.hotes[2], myIPort.ip.hotes[3],
            myIPort.port.iport);
    sprintf(hPort,"(%d.%d.%d.%d:%d)",
            hisIPort.ip.hotes[0], hisIPort.ip.hotes[1],
            hisIPort.ip.hotes[2], hisIPort.ip.hotes[3],
            hisIPort.port.iport);
}

IPPort* Socket::GetMyIPPort(){
    return &myIPort;
}

IPPort* Socket::GetClientIPPort(){
    return &hisIPort;
}

Bytes* Socket::ToStringMyIPPort(){
    return iPort;
}

Bytes* Socket::ToStringClientIPPort(){
    return hPort;
}
