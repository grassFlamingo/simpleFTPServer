#include <iostream>

#include "Socket.h"
#include "ThreadPool.h"
#include "FTPHandler.h"
#include "Session.h"
#include "setting.h"

int main()
{
    try{
        User::initUserMap();
        FTPHandler::initCmdFunctionList();
        Socket::init();
        Socket soc(AF_INET, SOCK_STREAM,IPPROTO_IP);
        soc.Bind(Setting::getInstance()->port);
        soc.Listen(16);
        ThreadPool pool(Setting::getInstance()->threadNumber);
        pool.startAll();
        printf("MY SOCKET(%s)\n", soc.ToStringMyIPPort());
        do{
            pool.push(soc.Accept());
        }while(1);
        pool.joinAll();
    }catch(Exception *e){
        printf("%s\n",e->getMessage());
        e->handeled();
        delete e;
    }
    return 0;
}
