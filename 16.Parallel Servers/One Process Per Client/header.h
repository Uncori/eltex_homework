#ifndef _HEADER_H
#define _HEADER_H

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIN_PORT 5005
#define BUFF_SIZE 1024
#define ERROR_RETURN -1

void checkRes(const int *res, const char *msg);
void sigChild(int sign);
int processWork(int clientFd);

#endif  // _HEADER_H
