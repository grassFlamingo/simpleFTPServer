#ifndef RWEXCEPTION_H
#define RWEXCEPTION_H

#include "exception.h"

class RwException: public Exception
{
public:
    RwException(char* message, int size);
    ~RwException();
};


#endif // RWEXCEPTION_H
