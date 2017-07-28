#ifndef SITE_H
#define SITE_H

#include "exception.h"
#include "Socket.h"
#include "Session.h"
#include "mystring.h"

class Site
{
public:
    Site();
    ~Site();

    Bytes* handel(Bytes* arg);
private:
    Bytes resp[200];
    Bytes* com;

private:
    void help();

};

#endif // SITE_H
