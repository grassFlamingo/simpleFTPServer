#ifndef MYSTRING_H
#define MYSTRING_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <memory.h>
#include <time.h>
#include <cstring>

typedef char Bytes;

class myString {
public:
static bool isEmpty(const Bytes* name);

static void toUpper(Bytes* str);

static int byteLen(const Bytes* byte);

};


//int get_file_size_time(const char *filename) {
//    struct stat statbuf;
//    if (stat(filename, &statbuf) == -1) {
//        printf("Get stat on %s Error：%s\n",
//               filename, strerror(errno));
//        return -1;
//    }
//    if ((statbuf.st_mode))return (1);
//    if (S_ISREG(statbuf.st_mode))
//        printf("%s size：%ld bytes\tmodified at %s",
//               filename, statbuf.st_size, ctime(&statbuf.st_mtime));
//    return 0;
//}
/*
struct dirent
{
    long  d_ino;              //inode number 索引节点号
    off_t d_off;              //offset to this dirent 在目录文件中的偏移
    unsigned short d_reclen;  //length of this d_name 文件名长
    unsigned char  d_type;    //the type of d_name 文件类型
    char  d_name[NAME_MAX+1]; //file name (null-terminated) 文件名，最长255字符
};

通过readdir函数读取到的文件名存储在结构体dirent的d_name成员中，而函数
int stat(const char *file_name, struct stat *buf);
的作用就是获取文件名为d_name的文件的详细信息，存储在stat结构体中。

struct stat {
    dev_t         st_dev;       //文件的设备编号 UINT
    ino_t         st_ino;       //节点
    mode_t        st_mode;      //文件的类型和存取的权限
    nlink_t       st_nlink;     //连到该文件的硬连接数目，刚建立的文件值为1
    uid_t         st_uid;       //用户ID
    gid_t         st_gid;       //组ID
    dev_t         st_rdev;      //(设备类型)若此文件为设备文件，则为其设备编号
    off_t         st_size;      //文件字节数(文件大小)
    unsigned long st_blksize;   //块大小(文件系统的I/O 缓冲区大小)
    unsigned long st_blocks;    //块数
    time_t        st_atime;     //最后一次访问时间
    time_t        st_mtime;     //最后一次修改时间
    time_t        st_ctime;     //最后一次改变时间(指属性)
};

*/
/*
 * -rw-rw-rw-   1 owner    group         105 May 16 08:05 setup.py
 * 第一段为-rwxrw-r--，
 * 第二段为1，
 * 第三段为user，
 * 第四段为group，
 * 第五段为文件长度，
 * 第六段为月，
 * 第七段为日，
 * 第八段为时间，
 * 第九段为文件名称。
*/

class myFileInfo{
public:
    Bytes* getInfo(Bytes* path);
    static long getFileSize(Bytes* path);
    static bool isDirectory(Bytes* path);
    static bool isExst(Bytes* path);
    static bool reName(Bytes* oldPath, Bytes* newPath);
private:
    void permition();
    void owner();
    void group();
    void toStrEnd();
    void lastMtime();
    int index;
    struct stat statBuf;
    Bytes Infomation[128];
};

#endif // MYSTRING_H
