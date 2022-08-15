#ifndef _HEADER_H
#define _HEADER_H


#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#define MESGSIZE 255
#define MAXCOUNTMESG 4
#define IPCNAME "/server"
#define FILEMOD 0777
#define MAXCLIENT 16
#define NICKNAMESIZE 55

typedef struct
{
    sem_t mutex;
    int countMsg;
    int countClient;
    char mesg[MESGSIZE];

} shmdata;

#endif  // _HEADER_H

