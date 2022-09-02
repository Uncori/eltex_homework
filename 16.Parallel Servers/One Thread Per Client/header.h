#ifndef _HEADER_H
#define _HEADER_H

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#define COUNT_CLIENT 10
#define SIN_PORT 5005
#define BUFF_SIZE 1024
#define ERROR_RETURN -1
#define BREAKER_CLIENT "!exit"

typedef struct {
  int fd;
  struct sockaddr_in addr;
  int id;
} sockInfo;

void checkRes(const int *res, const char *msg);

#endif  // _HEADER_H
