#include "header.h"

int main() {
  struct sockaddr_in servAddr, clientAddr;
  int sfd = 0, res = 0;
  ssize_t recvLen = 0, sendLen = 0;
  socklen_t len = 0;
  char buff[DATAGRAM_SIZE];

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

    recvLen = recvfrom(sfd, buff, DATAGRAM_SIZE, 0,
                       (struct sockaddr *)&clientAddr, &len);
    checkRes((int *)&recvLen, "recv");
    printf("|SERVER| - recvfrom complete: %s, recvfrom len: %d\n", buff,
           (int)recvLen);

    strncat(buff, " - message received", 20);

    sendLen = sendto(sfd, buff, DATAGRAM_SIZE, 0,
                     (struct sockaddr *)&clientAddr, len);
    printf("|SERVER| - sendto complete: %s, sendto len: %d\n\n", buff,
           (int)sendLen);

    memset(buff, 0, DATAGRAM_SIZE);
    memset(&clientAddr, 0, sizeof(struct sockaddr_in));
  }

  close(sfd);
  exit(EXIT_SUCCESS);
}