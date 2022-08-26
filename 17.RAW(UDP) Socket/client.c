#include "header.h"

void checkRes(const int *res, const char *sendBuff) {
  if (*res == ERROR_RETURN) {
    perror(sendBuff);
    exit(EXIT_FAILURE);
  }
}

int main() {
  struct sockaddr_in servAddr;
  socklen_t sockaddrLen = 0;
  int sfd = 0;
  ssize_t numBytes = 0;
  char recvBuff[DATAGRAMM_SIZE], datagram[DATAGRAMM_SIZE], message[BUFF_SIZE];
  char strIp[INET_ADDRSTRLEN];

  struct udphdr *udpHeader;

  sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  checkRes(&sfd, "socket error");
  printf("|CLIENT| - socket create\n");

  memset(&servAddr, 0, sizeof(struct sockaddr_in));
  memset(recvBuff, 0, sizeof(recvBuff));
  memset(datagram, 0, sizeof(datagram));
  memset(strIp, 0, sizeof(strIp));

  sockaddrLen = sizeof(struct sockaddr_in);

  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(SERVER_PORT);
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  printf("|CLIENT| - write massage: ");
  fgets(message, BUFF_SIZE, stdin);
  printf("message - %s\n", message);
  memcpy(datagram + sizeof(*udpHeader), message, sizeof(message));
  printf("datagram - %s\n", datagram);

  udpHeader = (struct udphdr *)datagram;
  udpHeader->source = htons(CLIENT_PORT);
  udpHeader->dest = htons(SERVER_PORT);
  udpHeader->check = 0;
  udpHeader->len = htons(strlen(message) - 1 + sizeof(struct udphdr));

  sendto(sfd, datagram, (sizeof(struct udphdr) + sizeof(message)), 0,
         (struct sockaddr *)&servAddr, sockaddrLen);
  printf("|CLIENT| - sendto complete : %s", datagram + sizeof(*udpHeader));

  while (1) {
    numBytes = recvfrom(sfd, recvBuff, sizeof(recvBuff), 0,
                        (struct sockaddr *)&servAddr, &sockaddrLen);
    checkRes((int *)&numBytes, "recvfrom");

    struct iphdr *ipHeaderRecv = (struct iphdr *)recvBuff;
    struct udphdr *udpHeaderRecv =
        (struct udphdr *)(recvBuff + sizeof(struct iphdr));

    inet_ntop(AF_INET, &ipHeaderRecv->saddr, strIp, INET_ADDRSTRLEN);

    if (!strncmp(strIp, "127.0.0.1", 10) &&
        (ntohs(udpHeaderRecv->dest) == CLIENT_PORT)) {
      printf("Message received: %s",
             (char *)(recvBuff + sizeof(struct iphdr) + sizeof(struct udphdr)));
      break;
    }
    memset(recvBuff, 0, DATAGRAMM_SIZE);
  }

  close(sfd);
  exit(EXIT_FAILURE);
}