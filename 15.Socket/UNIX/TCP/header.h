#ifndef _HEADER_H
#define _HEADER_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/my_socket_TCP"
#define BUFF_SIZE 256
#define ERROR_RETURN -1

#endif  // _HEADER_H
