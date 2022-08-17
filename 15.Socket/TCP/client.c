#include "header.h"

void checkRes(const int *res, const char *msg) {
  if (*res == ERROR_RETURN) {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
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

  for (int i = 1; i < argc; i++) {
    res = write(sfd, argv[i], strlen(argv[i]) + 1);
    checkRes(&res, "write error");
    printf("|CLIENT| - write complete\n");

    res = read(sfd, buff, BUFF_SIZE);
    checkRes(&res, "read error");
    printf("|CLIENT| - read complete\n");

    printf("|CLIENT| - msg sent:[%s]\n", argv[i]);
    printf("|CLIENT| - echo msg received: %s\n\n", buff);
  }

  strcpy(buff, "/exit");
  res = write(sfd, buff, BUFF_SIZE);
  checkRes(&res, "write error");
  printf("|CLIENT| - last write complete\n");

  close(sfd);
  exit(EXIT_FAILURE);
}