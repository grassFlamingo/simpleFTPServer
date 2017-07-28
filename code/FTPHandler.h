#ifndef FTPHANDLER_H
#define FTPHANDLER_H


#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <iterator>
#include <algorithm>

#include "exception.h"
#include "Socket.h"
#include "Session.h"
#include "mystring.h"
#include "site.h"

struct HandFun{
    void(*handle)(Session *session);
};

// A a;
////static成员函数,取得的是该函数在内存中的实际地址，而且因为static成员是全局的，所以不能用A::限定符
//void(*ptrstatic)()=&A::staticmember;
////nonstatic成员函数 取得的是该函数在内存中的实际地址
//void(A::*ptrnonstatic)()=&A::nonstatic;
////虚函数取得的是虚函数表中的偏移值，这样可以保证能过指针调用时同样的多态效果
//void(A::*ptrvirtual)()=&A::virtualmember;
////函数指针的使用方式
//ptrstatic();
//(a.*ptrnonstatic)();
//(a.*ptrvirtual)();

// the reason is that each command that turn to int is different
// min is 222 max is 334 diff 334 - 222 = 112


class FTPHandler
{
public:
    FTPHandler();
    ~FTPHandler();
    void exec(Socket* recvSocket);

    static void initCmdFunctionList();

    static void connectSucceed(Socket *soc);
    static void unAuthorize(Session* session);
    static void user(Session *session);
    static void pass(Session *session);
    static void list(Session *session);
    static void nlst(Session *session);
    static void pwd(Session *session);
    static void cwd(Session *session);
    static void type(Session *session);
    static void pasv(Session *session);
    static void port(Session *session);
    static void syst(Session *session);
    static void size(Session *session);
    static void quit(Session *session);
    static void stor(Session *session);
    static void retr(Session *session);
    static void noop(Session *session);
    static void dele(Session *session);
    static void mkd(Session *session);
    static void rmd(Session *session);
    static void rest(Session *session);
    static void error(Session *session);
    static void opts(Session *session);
    static void rnfr(Session *session);
    static void rnto(Session *session);
    static void site(Session *session);


private:
    static std::map< Ulong, HandFun > funMap;
};

#endif // FTPHANDLER_H
