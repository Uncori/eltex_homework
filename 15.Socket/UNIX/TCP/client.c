#include "header.h"

void checkRes(const int *res, const char *msg) {
  if (*res == ERROR_RETURN) {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}

int main() {
  int sfd = 0, res = 0;
  char buff[BUFF_SIZE];

  struct sockaddr_un server;
  server.sun_family = AF_LOCAL;
  strncpy(server.sun_path, SOCKET_PATH, sizeof(server.sun_path) - 1);

  sfd = socket(AF_LOCAL, SOCK_STREAM, 0);
  checkRes(&sfd, "socket error");
  printf("|CLIENT| - socket create\n");

  res = connect(sfd, (const struct sockaddr *)&server,
                sizeof(struct sockaddr_un));
  checkRes(&res, "connect error");
  printf("|CLIENT| - connect complete\n");
  printf("|CLIENT| - write massage: ");
  fgets(buff, BUFF_SIZE, stdin);

  res = write(sfd, buff, strlen(buff) - 1);
  checkRes(&res, "write error");
  printf("|CLIENT| - write complete\n");

  res = read(sfd, buff, BUFF_SIZE);
  checkRes(&res, "read error");
  printf("|CLIENT| - read complete\n");

  printf("|CLIENT| - msg sent: %s\n", buff);
  printf("|CLIENT| - echo msg received: %s\n", buff);

  close(sfd);
  exit(EXIT_FAILURE);
}