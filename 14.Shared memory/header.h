#ifndef _HEADER_H
#define _HEADER_H

#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MESGSIZE 255
#define MAXCOUNTMESG 4
#define IPCNAME "/server"
#define FILEMOD 0777
#define MAXCLIENT 16
#define NICKNAMESIZE 55

typedef struct {
  sem_t mutex;
  int countMsg;
  int countClient;
  char mesg[MESGSIZE];

} shmdata;

#endif  // _HEADER_H
