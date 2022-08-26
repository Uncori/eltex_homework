#include "header.h"

int main() {
  struct sockaddr_in servAddr, clientAddr;
  int sfd = 0, res = 0;
  ssize_t numBytes = 0;
  socklen_t len = 0;
  char buff[DATAGRAM_SIZE];
  char clientIp[INET_ADDRSTRLEN];

  sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  checkRes(&sfd, "socket error");
  printf("|SERVER| - socket create\n");

  memset(&servAddr, 0, sizeof(struct sockaddr_in));
  memset(&clientAddr, 0, sizeof(struct sockaddr_in));

  memset(buff, 0, DATAGRAM_SIZE);

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(SERVER_PORT);

  len = sizeof(struct sockaddr_in);

  res = bind(sfd, (struct sockaddr *)&servAddr, len);
  checkRes(&res, "bind error");
  printf("|SERVER| - bind complete\n");

  while (1) {
    len = sizeof(struct sockaddr_in);

    numBytes = recvfrom(sfd, buff, DATAGRAM_SIZE, 0,
                        (struct sockaddr *)&clientAddr, &len);
    checkRes((int *)&numBytes, "recv");
    printf("|SERVER| - recvfrom complete : %s\n", buff);

    strncat(buff, " - message received", 20);

    sendto(sfd, buff, DATAGRAM_SIZE, 0, (struct sockaddr *)&clientAddr, len);
    printf("|SERVER| - sendto complete : %s\n",
           buff + sizeof(struct sockaddr *));

    memset(buff, 0, DATAGRAM_SIZE);
    memset(&clientAddr, 0, sizeof(struct sockaddr_in));
  }

  close(sfd);
  exit(EXIT_SUCCESS);
}