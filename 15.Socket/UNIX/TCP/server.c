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
  struct sockaddr_un addr;

  sfd = socket(AF_LOCAL, SOCK_STREAM, 0);
  checkRes(&sfd, "socket error");
  printf("|SERVER| - socket create\n");

  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_LOCAL;
  strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

  res = bind(sfd, (const struct sockaddr *)&addr, sizeof(struct sockaddr_un));
  checkRes(&res, "bild error");
  printf("|SERVER| - bind complete\n");
  res = listen(sfd, 5);
  checkRes(&res, "listen error");
  printf("|SERVER| - listening clients\n");

  while (1) {
    cfd = accept(sfd, NULL, NULL);
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
    memset(buff, 0, BUFF_SIZE);
    close(cfd);
  }

  close(sfd);
  unlink(SOCKET_PATH);
  exit(EXIT_SUCCESS);
}