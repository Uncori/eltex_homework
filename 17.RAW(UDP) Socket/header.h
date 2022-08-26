#ifndef _HEADER_H
#define _HEADER_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define DATAGRAMM_SIZE 64

#define BUFF_SIZE 255

#define SERVER_PORT 5010
#define CLIENT_PORT 5020

#define ERROR_RETURN -1

#endif  // _HEADER_H
