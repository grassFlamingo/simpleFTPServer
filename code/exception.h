#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <memory.h>


class Exception
{
public:
    Exception(char* message, int size);
    ~Exception();

    void handeled();
    bool hasError();
    char* getMessage();

private:
    int size;
    char* message;

};

#endif // EXCEPTION_H
