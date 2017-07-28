#include "Session.h"


namespace User {

void initUserMap(){
    userMap["anonymous"] = "";
//    userMap["123"]       = "123";
    json jUser = Setting::getInstance()->user;
    // iterate the array
    for (json::iterator it = jUser.begin(); it != jUser.end(); it++) {
        userMap[(*it)["NAME"]] = (*it)["PASS"];
    }
}

bool hasUser(Bytes* name){
    std::map<std::string, std::string>::iterator it;
    std::string stdName(name);
    it = userMap.find(stdName);
    return it != userMap.end();
}

bool checkUser(Bytes* name, Bytes* password){
    if(myString::isEmpty(name)){
        return false;
    }
    std::string stdName(name);
    std::map<std::string, std::string>::iterator it = userMap.find(stdName);
    if(it == userMap.end()){
        return false;
    }
    std::string pass = it->second;
    return pass == "" ? true : pass == std::string(password);
}

}

const Bytes* Session::basicDir = Setting::getInstance()->dir.data();

Session::Session(Socket *request)
{
    authorize = false;
    type = SessionEnum::TUnset;
    mode = SessionEnum::MUnset;
    ReqSocket = request;
    TranferSocket = NULL;
    quit = false;
    offset = 0;
    setRelaticeDir("/");
    reNameDir[0] = '\0';
}

Session::~Session()
{

}

bool Session::loadRequest(Bytes* req){
    //    sscanf函数返回值的含义是有多少个变量被赋值。
    int ret = sscanf(req, "%s %[^\r\n]\r\n", cmd.name.bname, cmd.arg);
    myString::toUpper(cmd.name.bname);
    return ret > 0;
}

CmdName Session::getCmdName(){
    return cmd.name;
}

Bytes* Session::getArg(){
    return cmd.arg;
}

Command* Session::getCommand(){
    return &cmd;
}

void Session::Response(const Bytes* resp){
#ifdef DEBUG_MODE
    printf("response: %s",resp);
#endif
    ReqSocket->Send(resp);
    return;
}


void Session::TranferSocketSend(Bytes* mess){
    if(TranferSocket == NULL){
        return;
    }
//    printf("%s",mess);
    TranferSocket->Send(mess);
}
void Session::TranferSocketClose(){
    if(TranferSocket == NULL){
        return;
    }
    delete TranferSocket;
    TranferSocket = NULL;
}

void Session::setUser(Bytes* name){
    memcpy(user, name, sizeof(Bytes)*(myString::byteLen(name) + 1));
}

Bytes* Session::getUser(){
    return user;
}

bool Session::authorizeUser(Bytes* name, Bytes* password){
    authorize = User::checkUser(name, password);
    return authorize;
}

bool Session::isAuthrized(){
    return authorize;
}


Socket* Session::getReqSocket(){
    return ReqSocket;
}

Socket* Session::getTranferSocket(){
    return TranferSocket;
}

void Session::setTranferSocket(Socket* pasv){
    if(TranferSocket){
        delete TranferSocket;
    }
    TranferSocket = pasv;
}

Bytes* Session::getGobalDir(){
    return goDir;
}

void Session::setRelaticeDir(const Bytes* dir){
    if(dir[0] == '/'){
        // root dir
        strcpy(relativeDir,dir);
    }else{
        sprintf(relativeDir,"%s/%s",relativeDir,dir);
    }
    int len = strlen(relativeDir);
    if(len > 0 && relativeDir[len-1] == '/' ){
        relativeDir[len-1] = '\0';
    }
    sprintf(goDir,"%s%s",basicDir,relativeDir);
}

void Session::setPreRenameDir(const Bytes *dir){
    sprintf(reNameDir,"%s",dir);
}

Bytes* Session::getPreNameDir(){
    return reNameDir;
}

Bytes* Session::getRelativeDir(){
    return relativeDir;
}

int Session::getPath(Bytes *recever, Bytes *arg){
    int succ;
    if(arg[0] == '/'){
        succ = sprintf(recever,"%s%s",basicDir,arg);
    }else{
        succ = sprintf(recever,"%s/%s",goDir,arg);
    }
    return succ;
}
