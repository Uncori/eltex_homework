#include "header.h"

void checkRes(const int *res, const char *msg) {
  if (*res == ERROR_RETURN) {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}

int main() {
  struct sockaddr_in servAddr, clientAddr;
  int sfd = 0, res = 0;
  ssize_t numBytes = 0;
  socklen_t len = 0;
  char buff[BUFF_SIZE];

  sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  checkRes(&sfd, "socket error");
  printf("|SERVER| - socket create\n");

  memset(&servAddr, 0, sizeof(struct sockaddr_in));
  memset(&clientAddr, 0, sizeof(struct sockaddr_in));

  memset(buff, 0, sizeof(buff));

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(SERVER_PORT);

  res = bind(sfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr_in));
  checkRes(&res, "bind error");
  printf("|SERVER| - bind complete\n");

  while (1) {
    len = sizeof(struct sockaddr_in);

    numBytes = recvfrom(sfd, buff, sizeof(buff), 0,
                        (struct sockaddr *)&clientAddr, &len);
    printf("|SERVER| - recvfrom complete : %s\n", buff);

    if (numBytes == -1) {
      perror("recvfrom error");
      close(sfd);
      exit(EXIT_FAILURE);
    }

    if (!strncmp(buff, "!server off", 12)) {
      break;
    }

    strncat(buff, " - message received", 20);

    sendto(sfd, buff, sizeof(buff), 0, (struct sockaddr *)&clientAddr, len);
    printf("|SERVER| - sendto complete : %s\n", buff);

    memset(buff, 0, sizeof(buff));
    memset(&clientAddr, 0, sizeof(struct sockaddr_in));
  }

  close(sfd);
  exit(EXIT_SUCCESS);
}