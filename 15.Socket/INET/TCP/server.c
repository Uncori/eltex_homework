#include "header.h"

void checkRes(const int *res, const char *msg) {
  if (*res == ERROR_RETURN) {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}

int main() {
  int sfd = 0, cfd = 0, res = 0;
  char buff[BUFF_SIZE];
  struct sockaddr_in servAddr;

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  checkRes(&sfd, "socket error");
  printf("|SERVER| - socket ready\n");

  memset(&servAddr, 0, sizeof(struct sockaddr_in));
  memset(buff, 0, sizeof(buff));

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(SIN_PORT);

  res =
      bind(sfd, (const struct sockaddr *)&servAddr, sizeof(struct sockaddr_in));
  checkRes(&res, "bild error");
  printf("|SERVER| - bind complete\n");
  res = listen(sfd, 5);
  checkRes(&res, "listen error");
  printf("|SERVER| - listening clients\n");

  while (1) {
    cfd = accept(sfd, (struct sockaddr *)NULL, NULL);
    checkRes(&cfd, "accept error");
    printf("|SERVER| - Accept\n");

    res = recv(cfd, buff, BUFF_SIZE, 0);
    checkRes(&res, "recv error");
    printf("|SERVER| - recv complete : %s\n", buff);

    res = send(cfd, buff, BUFF_SIZE, 0);
    checkRes(&res, "send error");
    printf("|SERVER| - echo recv complete\n");

    if (!strncmp(buff, "!server off", 12)) {
      close(cfd);
      break;
    }
    memset(buff, 0, sizeof(buff));
    close(cfd);
  }

  close(sfd);
  exit(EXIT_SUCCESS);
}