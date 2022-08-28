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
    printf(
        "|CLIENT| - server IP: %s\n\t\tsourse port: %d\n\t\t\tdest port: %d\n",
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
  struct iphdr *ipHeader;

  message = calloc(DATAGRAM_SIZE - sockaddrLen, sizeof(char));
  printf("|CLIENT| - write massage: ");
  fgets(message, DATAGRAM_SIZE - sockaddrLen, stdin);

  datagram = calloc(DATAGRAM_SIZE, sizeof(char));
  memmove(datagram + sizeof(struct iphdr) + sizeof(struct udphdr), message,
          strlen(message) - 1);

  ipHeader = (struct iphdr *)datagram;
  ipHeader->ihl = 5;
  ipHeader->version = 4;
  ipHeader->tos = 0;
  ipHeader->id = htonl(601);
  ipHeader->frag_off = 0;
  ipHeader->ttl = 255;
  ipHeader->protocol = IPPROTO_UDP;
  ipHeader->saddr = INADDR_ANY;
  ipHeader->daddr = servAddr.sin_addr.s_addr;

  udpHeader = (struct udphdr *)(datagram + sizeof(struct iphdr));
  udpHeader->source = htons(CLIENT_PORT);
  udpHeader->dest = htons(SERVER_PORT);
  udpHeader->check = 0;
  udpHeader->len = htons(strlen(message) - 1 + sizeof(struct udphdr));

  sentLen = sendto(socketFd, datagram, DATAGRAM_SIZE, 0,
                   (struct sockaddr *)&servAddr, sockaddrLen);
  checkRes((int *)&sentLen, "sendto");
  printf("|CLIENT| - sendto complete : %d\n", (int)sentLen);

  while (breaker) {
    breaker = datagramRecv(socketFd, recvLen, datagram, sockaddrLen, servAddr,
                           udpHeader);
  }
  free(message);
  free(datagram);
}
