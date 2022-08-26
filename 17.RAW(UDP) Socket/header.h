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

#define DATAGRAM_SIZE 255
#define SERVER_PORT 5010
#define CLIENT_PORT 5020
#define ERROR -1

void datagramSent(int socketFd, struct sockaddr_in servAddr);
void checkRes(const int *res, const char *sendBuff);
int datagramRecv(int socketFd, ssize_t recvLen, char *datagram,
                 socklen_t sockaddrLen, struct sockaddr_in servAddr,
                 struct udphdr *udpHeader);

#endif  // _HEADER_H
