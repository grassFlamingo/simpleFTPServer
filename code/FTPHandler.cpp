#include "FTPHandler.h"

FTPHandler::FTPHandler()
{
    //ctor
}

FTPHandler::~FTPHandler()
{
    //dtor
}

std::map< Ulong, HandFun > FTPHandler::funMap;

void FTPHandler::initCmdFunctionList(){
    //    const static Bytes *allcmd[]{
    //        "USER","PASS","ACCT","REIN","QUIT","PORT","PASV",
    //        "TYPE","STRU","MODE","RETR","STOR","APPE","MLFL",
    //        "MAIL","MSND","MSOM","MSAM","MRSQ","MRCP","ALLO",
    //        "REST","RNFR","RNTO","ABOR","DELE","CWD" ,"LIST",
    //        "NLST","SITE","STAT","HELP","NOOP",""
    //    };
    funMap[CmdName{"USER"}.lname]   = HandFun{&FTPHandler::user};
    funMap[CmdName{"PASS"}.lname]   = HandFun{&FTPHandler::pass};
    funMap[CmdName{"PWD"}.lname]    = HandFun{&FTPHandler::pwd};
    funMap[CmdName{"LIST"}.lname]   = HandFun{&FTPHandler::list};
    funMap[CmdName{"NLST"}.lname]   = HandFun{&FTPHandler::nlst};
    funMap[CmdName{"TYPE"}.lname]   = HandFun{&FTPHandler::type};
    funMap[CmdName{"PASV"}.lname]   = HandFun{&FTPHandler::pasv};
    funMap[CmdName{"PORT"}.lname]   = HandFun{&FTPHandler::port};
    funMap[CmdName{"SYST"}.lname]   = HandFun{&FTPHandler::syst};
    funMap[CmdName{"SIZE"}.lname]   = HandFun{&FTPHandler::size};
    funMap[CmdName{"CWD"}.lname]    = HandFun{&FTPHandler::cwd};
    funMap[CmdName{"QUIT"}.lname]   = HandFun{&FTPHandler::quit};
    funMap[CmdName{"STOR"}.lname]   = HandFun{&FTPHandler::stor};
    funMap[CmdName{"RETR"}.lname]   = HandFun{&FTPHandler::retr};
    funMap[CmdName{"NOOP"}.lname]   = HandFun{&FTPHandler::noop};
    funMap[CmdName{"DELE"}.lname]   = HandFun{&FTPHandler::dele};
    funMap[CmdName{"MKD"}.lname]    = HandFun{&FTPHandler::mkd};
    funMap[CmdName{"RMD"}.lname]    = HandFun{&FTPHandler::rmd};
    funMap[CmdName{"REST"}.lname]   = HandFun{&FTPHandler::rest};
    funMap[CmdName{"OPTS"}.lname]   = HandFun{&FTPHandler::opts};
    funMap[CmdName{"RNFR"}.lname]   = HandFun{&FTPHandler::rnfr};
    funMap[CmdName{"RNTO"}.lname]   = HandFun{&FTPHandler::rnto};
    funMap[CmdName{"SITE"}.lname]   = HandFun{&FTPHandler::site};

}

void FTPHandler::connectSucceed(Socket *soc){
    soc->Send("220 cftpdlib based ftpd ready.\r\n");
}

void FTPHandler::exec(Socket *recvSocket){
    Session session(recvSocket);
    while(recvSocket->Recv() && !session.quit){
        session.loadRequest(recvSocket->GetRecvData());
#ifdef DEBUG_MODE
        printf("recv: %s",recvSocket->GetRecvData());
#endif
        Command* cmd = session.getCommand();
        std::map< Ulong, HandFun >::iterator it = funMap.find(cmd->name.lname);
        if(it == funMap.end()){
            error(&session);
            return;
        }
        if(!session.isAuthrized() &&
            !(it->second.handle == &FTPHandler::user ||
              it->second.handle == &FTPHandler::pass ||
              it->second.handle == &FTPHandler::quit )){
            unAuthorize(&session);
            continue;
        }
        it->second.handle(&session);
    }
}

void FTPHandler::user(Session *session){
    Bytes* arg = session->getArg();
    if(myString::isEmpty(arg)){
        session->Response("530 Log in with USER and PASS first.\r\n");
    }else{
        session->Response("331 Username ok, send password.\r\n");
        session->setUser(arg);
    }
}

void FTPHandler::pass(Session *session){
    Bytes* arg = session->getArg();
    if(session->isAuthrized()){
        session->Response("503 User already authenticated.\r\n");
        return;
    }
    printf("user %s password %s\n", session->getUser(), arg);
    if(session->authorizeUser(session->getUser(), arg)){
        session->Response("230 Login successful.\r\n");
    }else{
        session->Response("530 Authentication failed.\r\n");
    }
}

void FTPHandler::unAuthorize(Session *session){
    session->Response("530 Log in with USER and PASS first.\r\n");
}

void FTPHandler::list(Session *session){
    DIR* dir = opendir(session->getGobalDir());
    session->Response("125 Data connection already open. Transfer starting.\r\n");
    char bufer[BUFFER_SIZE];
    dirent *entp;
    myFileInfo myfl;
    Bytes fileDir[256];
    while ((entp = readdir(dir)) != NULL){
        if(entp->d_name[0] == '.'){
            continue;
        }
        sprintf(fileDir, "%s/%s",session->getGobalDir(),entp->d_name);
        sprintf(bufer, "%s %s\r\n",myfl.getInfo(fileDir),
                entp->d_name);
        session->TranferSocketSend(bufer);
    }
    closedir(dir);
    session->TranferSocketClose();
    session->Response("226 Transfer complete.\r\n");
}

void FTPHandler::nlst(Session *session){
    Socket* tSock = session->getTranferSocket();
    DIR* dir = opendir(session->getGobalDir());
    session->Response("125 Data connection already open. Transfer starting.\r\n");
    char bufer[BUFFER_SIZE];
    dirent *direntp;
    while ((direntp = readdir(dir)) != NULL){
        if(direntp->d_name[0] == '.'){
            continue;
        }
        sprintf(bufer, "%s\r\n",direntp->d_name );
//        puts(bufer);
        tSock->Send(bufer);
    }
    closedir(dir);
    session->TranferSocketClose();
    session->Response("226 Transfer complete.\r\n");
}

void FTPHandler::type(Session *session){
    Bytes* arg = session->getArg();
    switch (arg[0]) {
    case 'A':
    case 'a':
        session->type = SessionEnum::ASCII;
        session->Response("200 Type set to: ASCII.\r\n");
        break;
    case 'I':
    case 'i':
        session->type = SessionEnum::Binary;
        session->Response("200 Type set to: Binary.\r\n");
        break;
    default:
        session->Response("504 Command not implemented for that parameter.\r\n");
        break;
    }
}

void FTPHandler::pasv(Session *session){
    Socket soc(AF_INET, SOCK_STREAM,IPPROTO_IP);
    IPAddress ip = session->getReqSocket()->GetMyIPPort()->ip;
    soc.Bind(ip,0);
    soc.Listen(2);
    Port p = soc.GetMyIPPort()->port;
    Bytes send[100];
    sprintf(send, "227 Entering passive mode (%d,%d,%d,%d,%d,%d).\r\n",
            ip.hotes[0], ip.hotes[1], ip.hotes[2], ip.hotes[3],
            p.cport[1], p.cport[0]);
    session->Response(send);
    session->setTranferSocket(soc.Accept());
    session->mode = SessionEnum::PASV;
}

void FTPHandler::port(Session *session){
    Bytes* arg = session->getArg();
    IPPort iport;
    UINT i0,i1,i2,i3,p0,p1;
    sscanf(arg,"%d,%d,%d,%d,%d,%d",&i0,&i1,&i2,&i3,&p0,&p1);
    iport.ip.hotes[0]   = i0;
    iport.ip.hotes[1]   = i1;
    iport.ip.hotes[2]   = i2;
    iport.ip.hotes[3]   = i3;
    iport.port.cport[1] = p0;
    iport.port.cport[0] = p1;
    Socket* soc = new Socket(AF_INET, SOCK_STREAM,IPPROTO_IP);
    soc->Connect(iport.ip,iport.port.iport);
    session->setTranferSocket(soc);
    session->Response("200 Active data connection established.\r\n");
    session->mode = SessionEnum::PORT;
}

void FTPHandler::pwd(Session *session){
    Bytes tsp[200];
    Bytes* relDir = session->getRelativeDir();
    if(relDir[0] == '/'){
        sprintf(tsp,"257 \"%s\" is the current directory.\r\n",relDir);
    }else{
        sprintf(tsp, "257 \"/\" is the current directory.\r\n");
    }
    session->Response(tsp);
}

void FTPHandler::cwd(Session *session){
    Bytes* arg = session->getArg();
    Bytes tsp[256];
    session->getPath(tsp,arg);
    if(myFileInfo::isDirectory(tsp)){
        session->setRelaticeDir(arg);
        sprintf(tsp,"250 \"%s\" is the current directory.\r\n",session->getRelativeDir());
        session->Response(tsp);
    }else{
        session->Response("550 Not a directory.\r\n");
    }
}

void FTPHandler::syst(Session *session){
    session->Response("215 UNIX Type: L8\r\n");
}

void FTPHandler::size(Session *session){
    //    213 3703
    Bytes* arg = session->getArg();
    Bytes fileDir[256];
    session->getPath(fileDir,arg);
    long fsize = myFileInfo::getFileSize(fileDir);
    if(fsize <= 0){
        sprintf(fileDir,"550 %s is not retrievable.\r\n", arg);
    }else{
        sprintf(fileDir,"213 %lu\r\n",fsize);
    }
    session->Response(fileDir);
}

void FTPHandler::quit(Session *session){
    session->quit = true;
    session->Response("221 Goodbye.\r\n");
}

// upload
void FTPHandler::stor(Session *session){
    Bytes* arg = session->getArg();
    Bytes stp[256];
    session->getPath(stp,arg);
    printf("User %s %s upload %s\n",
           session->getUser(),
           session->getReqSocket()->ToStringClientIPPort(),
           stp);
    FILE* file = fopen(stp,"wb");
    if(file == NULL){
        session->Response("550 No such file or directory.\r\n");
        return;
    }
    session->Response("125 Data connection already open. Transfer starting.\r\n");
    Socket* tSoc = session->getTranferSocket();
    if(tSoc == NULL){
        tSoc = session->getReqSocket();
    }
    int size = 1024;
    while(tSoc->Recv(&size)){
        fwrite(tSoc->GetRecvData(), size, 1, file);
    }
    fclose(file);
    session->TranferSocketClose();
    session->Response("226 Transfer complete.\r\n");
}

// download
void FTPHandler::retr(Session *session){
    //RETR a.in\r\n
    //125 Data connection already open. Transfer starting.\r\n
    //550 No such file or directory.\r\n
    Bytes* arg = session->getArg();
    Bytes stp[256];
    session->getPath(stp,arg);
    printf("User %s %s download %s\n",
           session->getUser(),
           session->getReqSocket()->ToStringClientIPPort(),
           stp);
    FILE* file = fopen(stp,"rb");
    if(file == NULL){
        session->Response("550 No such file or directory.\r\n");
        return;
    }
    Bytes buf[1024];
    //     fread(array, sizeof(struct record), 2, fp);
    Socket* tSoc = session->getTranferSocket();
    if(tSoc == NULL){
        tSoc = session->getReqSocket();
    }
    session->Response("125 Data connection already open. Transfer starting.\r\n");
    int size;
    try{
        fseek(file, session->offset, SEEK_SET);
        while( (size = fread(buf, 1, 1000, file) ) > 0){
            tSoc->Send(buf,size);
        }
    }catch(RwException* e){
        printf("%s\n",e->getMessage());
        e->handeled();
        delete e;
    }
    fclose(file);
    session->TranferSocketClose();
    session->Response("226 Transfer complete.\r\n");
}

void FTPHandler::noop(Session *session){
    session->Response("200 I successfully done nothing.\r\n");
}

void FTPHandler::dele(Session *session){
    Bytes* arg = session->getArg();
    Bytes tsp[256];
    session->getPath(tsp,arg);
    printf("User %s %s delete %s\n",
           session->getUser(),
           session->getReqSocket()->ToStringClientIPPort(),
           tsp);
    if(remove(tsp) == 0){
        session->Response("250 File removed.\r\n");
    }else{
        session->Response("550 No such file or directory.\r\n");
    }
}

void FTPHandler::mkd(Session *session){
    Bytes* arg = session->getArg();
    Bytes tsp[256];
    session->getPath(tsp,arg);
    printf("User %s %s make dir %s\n",
           session->getUser(),
           session->getReqSocket()->ToStringClientIPPort(),
           tsp);
    if(myFileInfo::isDirectory(tsp)){
        session->Response("550 File exists.\r\n");
        return;
    }
    if(mkdir(tsp) == 0){
        sprintf(tsp,"257 \"%s\" directory created.\r\n",arg);
        session->Response(tsp);
    }else{
        session->Response("550 Invalid argument.\r\n");
    }
}

void FTPHandler::rmd(Session *session){
    Bytes* arg = session->getArg();
    Bytes tsp[256];
    session->getPath(tsp,arg);
    printf("User %s %s remove dir %s\n",
           session->getUser(),
           session->getReqSocket()->ToStringClientIPPort(),
           tsp);
    printf("rmd tsp %s\n",tsp);
    if(!myFileInfo::isDirectory(tsp)){
        session->Response("550 No such file or directory.\r\n");
        return;
    }
    if(rmdir(tsp) == 0){
        session->Response("250 Directory removed.\r\n");
    }else{
        session->Response("550 Can not remove this file or directory.\r\n");
    }
}

void FTPHandler::rest(Session *session){
    Bytes* arg = session->getArg();
    sscanf(arg,"%lu",&session->offset);
    Bytes tsp[100];
    sprintf(tsp,"350 Restarting at position %lu.\r\n",session->offset);
    session->Response(tsp);
}

void FTPHandler::error(Session *session){
    Bytes *arg = session->getArg();
    Bytes *cmd = session->getCmdName().bname;
    Bytes resp[100];
    sprintf(resp, "500 '%s %s': command not understood.\r\n", cmd, arg);
    session->Response(resp);
}

void FTPHandler::opts(Session *session){
//    opts utf8 on
//    501 UTF8 not supported is invalid.
    Bytes* arg = session->getArg();
    Bytes resp[100];
    sprintf(resp,"501 %s not supported is invalid.\r\n", arg);
    session->Response(resp);
}

void FTPHandler::rnfr(Session *session){
    Bytes* arg = session->getArg();
    Bytes tsp[256];
    session->getPath(tsp,arg);
    if(myFileInfo::isExst(tsp)){
        session->setPreRenameDir(arg);
        session->Response("350 Ready for destination name.\r\n");
    }else{
        session->Response("550 No such file or directory.\r\n");
    }
}

void FTPHandler::rnto(Session *session){
    Bytes* oldName = session->getPreNameDir();
    if(oldName[0] == '\0'){
        session->Response("502 Command not implemented.\r\n");
        return;
    }
    Bytes* arg = session->getArg();
    Bytes oldPath[256],newPath[256];
    session->getPath(oldPath,oldName);
    session->getPath(newPath,arg);
    if(myFileInfo::reName(oldPath, newPath)){
        session->Response("250 Renaming ok.\r\n");
    }else{
        session->Response("502 Command not implemented.\r\n");
    }
}

void FTPHandler::site(Session *session){
    session->Response(Site().handel(session->getArg()));
}
