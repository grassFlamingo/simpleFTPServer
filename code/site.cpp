#include "site.h"

Site::Site()
{
    strcpy(resp,"501 command are unrecognized\r\n");
}

Site::~Site(){

}

Bytes *Site::handel(Bytes *arg){
    com = arg;
    if(strcmp(arg,"help") == 0){
        help();
    }
    return &resp[0];
}

void Site::help(){
    strcpy(resp,
           "214-The following SITE commands are recognized (* =>'s unimplemented)\r\n"
           "HELP\r\n"
           "214 Direct comments to cftpdlib\r\n");
}


