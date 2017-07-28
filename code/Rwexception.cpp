#include "Rwexception.h"

#include <stdio.h>

RwException::RwException(char *message, int size) :
    Exception(message, size)
{

}

RwException::~RwException(){

}
