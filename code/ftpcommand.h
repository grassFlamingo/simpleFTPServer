#ifndef FTPCOMMAND_H
#define FTPCOMMAND_H

#define CRLF \r\n

typedef char Bytes;

typedef struct{
    union{
        Bytes bname[4];
        unsigned int iname;
    }name;
    struct{
        Bytes arg[128];
    }Arg;
    Bytes* message;
}Command;

class FTPCommand
{
public:
    FTPCommand();
    ~FTPCommand();



};

#endif // FTPCOMMAND_H
