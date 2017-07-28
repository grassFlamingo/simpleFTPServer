#include "exception.h"

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

Exception::Exception(char *message, int size)
{
    this->message = new char[size];
    memcpy(this->message, message, size);
    this->size = size;
}

Exception::~Exception(){
    if(message){
        delete[] message;
    }
}

void Exception::handeled(){
    if(message){
        delete[] message;
    }
    message = NULL;
}

bool Exception::hasError(){
    return message == NULL;
}

char* Exception::getMessage(){
    return message;
}
