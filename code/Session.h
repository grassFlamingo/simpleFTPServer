#ifndef Session_H
#define Session_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <iterator>

#include "mystring.h"
#include "Socket.h"
#include "setting.h"

#define RESPONSE_SIZE 1024

#define CRLF \r\n

typedef unsigned long Ulong;
typedef char Bytes;

union CmdName{
    Bytes bname[8]; // 8 bytes
    Ulong lname; // 8 bytes
};

typedef struct{
    CmdName name;
    Bytes arg[256]; // 32 bytes
}Command;

namespace User {
extern void initUserMap();
extern bool hasUser(Bytes* name);
extern bool checkUser(Bytes* name, Bytes* password);
static std::map<std::string, std::string> userMap;
}

namespace SessionEnum {
enum FTPType{
    TUnset,
    ASCII,
    Binary
};

enum FTPMode{
    MUnset,
    PORT,
    PASV
};

}


class Session
{
public:
    Session(Socket* request);
    ~Session();

    bool loadRequest(Bytes* req);

    CmdName getCmdName();
    Bytes* getArg();
    Command *getCommand();

    void Response(const Bytes *resp);

    bool authorizeUser(Bytes* name, Bytes* password);
    bool isAuthrized();

    void setUser(Bytes* name);
    Bytes* getUser();

    Socket* getReqSocket();

    Socket* getTranferSocket();
    void setTranferSocket(Socket* resp);
    void TranferSocketSend(Bytes* mess);
    void TranferSocketClose();

    bool quit;

    SessionEnum::FTPType type;
    SessionEnum::FTPMode mode;

    Bytes* getGobalDir();

    void setRelaticeDir(const Bytes *dir);
    Bytes* getRelativeDir();
    unsigned long offset;
    void setPreRenameDir(const Bytes *dir);
    Bytes* getPreNameDir();
    int getPath(Bytes* recever, Bytes* arg);

private:
    Socket* ReqSocket;
    Socket* TranferSocket;

    Bytes user[16];
    Command cmd;

    bool authorize;
    Bytes relativeDir[256];
    Bytes goDir[256];
    Bytes reNameDir[256];
    static const Bytes* basicDir;

};

#endif // Session_H
