#include "mystring.h"

bool myString::isEmpty(const Bytes* name){
    for( int i = 0; name[i] != '\0'; i++){
        if(name[i] != ' ' && name[i] != '\t' ){
            return false;
        }
    }
    return true;
}

void myString::toUpper(Bytes* str){
    for( int i = 0; str[i] != '\0'; i++){
        if(str[i] >= 'a' && str[i] <= 'z'){
            str[i] = str[i] - 32;
        }
    }
}

int myString::byteLen(const Bytes* byte){
    int i = 0;
    while(byte[i++] != '\0');
    return i-1;
}

//struct tm {
//  int tm_sec;
//  int tm_min;
//  int tm_hour;
//  int tm_mday;
//  int tm_mon;
//  int tm_year;
//  int tm_wday;
//  int tm_yday;
//  int tm_isdst;
//};
const static char* MONTH[] = {
    "Jan", "Feb", "Mar",
    "Apr", "May", "Jun",
    "Jul", "Aug", "Sept",
    "Oct", "Nov", "Dec"
};

Bytes* myFileInfo::getInfo(Bytes* path){
    stat(path, &statBuf);
//    printf("getInfo path %s\n", path);
    index = 0;
    memset(Infomation,0,sizeof(Infomation));
    permition();
    tm *ltm = localtime(&statBuf.st_mtime);
//    drwx------ 3 slacker  users         104 Jul 27 01:45 public_html
    sprintf(&Infomation[index], "%d owner group %20lu %4s %02d %02d:%02d",
            statBuf.st_nlink,statBuf.st_size,
            MONTH[ltm->tm_mon], ltm->tm_mday, ltm->tm_hour, ltm->tm_min);
    //ltm->tm_year + 1900
    // the tm->tm_year was begin from 1900
    toStrEnd();
    return Infomation;
}

/*
 * S_ISREG() 测试是否为普通文件
 * S_ISDIR() 测试是否为目录文件
 * S_ISCHR() 测试是否为字符特殊文件
 * S_ISBLK() 测试是否为块特殊文件
 * S_ISFIFO() 测试是否为FIFO文件
 * S_ISLNK() 测试是否为链接文件
 * S_ISSOCK() 测试是否为socket文件
 * S_ISUID() 测试是否设置了“设置-用户-ID”位
 * S_ISGID() 测试是否设置了“设置-组-ID”位
 * S_IRUSR 所有者-读
 * S_IWUSR 所有者-写
 * S_IXUSR 所有者-执行
 * S_IRGRP 组-读
 * S_IWGRP 组-写
 * S_IXGRP 组-执行
 * S_IROTH 其他-读
 * S_IWOTH 其他-写
 * S_IXOTH 其他-执行
 */
void myFileInfo::permition(){
    unsigned short mode = statBuf.st_mode;
    Infomation[index++] = ( mode & S_IFMT ) == S_IFDIR ? 'd' : '-';
    Infomation[index++] = ( mode & S_IRUSR ) ? 'r' : '-';
    Infomation[index++] = ( mode & S_IWUSR ) ? 'w' : '-';
    Infomation[index++] = ( mode & S_IXUSR ) ? 'x' : '-';
    Infomation[index++] = ( mode & S_IRGRP ) ? 'r' : '-';
    Infomation[index++] = ( mode & S_IWGRP ) ? 'w' : '-';
    Infomation[index++] = ( mode & S_IXGRP ) ? 'x' : '-';
    Infomation[index++] = ( mode & S_IROTH ) ? 'r' : '-';
    Infomation[index++] = ( mode & S_IWOTH ) ? 'w' : '-';
    Infomation[index++] = ( mode & S_IXOTH ) ? 'x' : '-';
    Infomation[index++] = ' ';
}

void myFileInfo::toStrEnd(){
    while(Infomation[index++] != '\0');
}

long myFileInfo::getFileSize(Bytes *path){
    struct stat buf;
    stat(path, &buf);
    return buf.st_size;
}

bool myFileInfo::isDirectory(Bytes *path){
//    #include<dirent.h>
//    printf("is DIR path %s\n", path);
    struct stat buf;
    stat(path, &buf);
    return ( buf.st_mode & S_IFMT ) == S_IFDIR;
}

bool myFileInfo::isExst(Bytes *path){
    return access(path,0) == 0;
}

bool myFileInfo::reName(Bytes *oldPath, Bytes *newPath){
    return rename(oldPath,newPath) == 0;
}
