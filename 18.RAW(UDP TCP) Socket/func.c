#include "header.h"

void checkRes(const int *res, const char *sendBuff) {
  if (*res == ERROR) {
    perror(sendBuff);
    exit(EXIT_FAILURE);
  }
}

int datagramRecv(int socketFd, ssize_t recvLen, char *datagram,
                 socklen_t sockaddrLen, struct sockaddr_in servAddr,
                 struct udphdr *udpHeader) {
  int error = 1;
  recvLen = recvfrom(socketFd, datagram, DATAGRAM_SIZE, 0,
                     (struct sockaddr *)&servAddr, &sockaddrLen);
  checkRes((int *)&recvLen, "recvfrom");

  struct iphdr *ipHeader = (struct iphdr *)(char *)datagram;
  udpHeader = (struct udphdr *)(datagram + sizeof(struct iphdr));

  if ((ntohs(udpHeader->dest) == CLIENT_PORT)) {
    struct in_addr sAddr;
    sAddr.s_addr = ipHeader->saddr;
    printf("|CLIENT| - server IP: %s, sourse port: %d, dest port: %d\n",
           inet_ntoa(sAddr), ntohs(udpHeader->source), ntohs(udpHeader->dest));
    printf("|CLIENT| - recvfrom len: %d\n", (int)recvLen);

    printf("|CLIENT| - message: %s\n",
           datagram + sizeof(struct iphdr) + sizeof(struct udphdr));

    error = 0;
  }
  return error;
}

void datagramSent(int socketFd, struct sockaddr_in servAddr) {
  socklen_t sockaddrLen = 0;
  sockaddrLen = sizeof(struct sockaddr_in);

  int breaker = 1;
  ssize_t sentLen = 0, recvLen = 0;
  char *message, *datagram;

  struct udphdr *udpHeader;

  message = calloc(DATAGRAM_SIZE - sockaddrLen, sizeof(char));
  printf("|CLIENT| - write massage: ");
  fgets(message, DATAGRAM_SIZE - sockaddrLen, stdin);

  datagram = calloc(DATAGRAM_SIZE, sizeof(char));
  memmove(datagram + 8, message, strlen(message) - 1);

  udpHeader = (struct udphdr *)datagram;
  udpHeader->source = htons(CLIENT_PORT);
  udpHeader->dest = htons(SERVER_PORT);
  udpHeader->check = 0;
  udpHeader->len = htons(strlen(message) - 1 + sizeof(struct udphdr));

  sentLen = sendto(socketFd, datagram, DATAGRAM_SIZE, 0,
                   (struct sockaddr *)&servAddr, sockaddrLen);
  checkRes((int *)&sentLen, "sendto");
  printf("|CLIENT| - sendto complete : %d", (int)sentLen);

  while (breaker) {
    breaker = datagramRecv(socketFd, recvLen, datagram, sockaddrLen, servAddr,
                           udpHeader);
  }
  free(message);
  free(datagram);
}
