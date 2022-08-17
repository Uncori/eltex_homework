#include "header.h"

void checkRes(const int *res, const char *msg) {
  if (*res == ERROR_RETURN) {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}

int main() {
  int sfd = 0, cfd = 0, res = 0;
  struct sockaddr_un server;
  server.sun_family = AF_LOCAL;
  strncpy(server.sun_path, SOCKET_PATH, sizeof(server.sun_path) - 1);

  sfd = socket(AF_LOCAL, SOCK_STREAM, 0);
  checkRes(&sfd, "socket error");
  printf("|SERVER| - socket create\n");

  res = bind(sfd, (const struct sockaddr *)&server, sizeof(struct sockaddr_un));
  checkRes(&res, "bild error");
  printf("|SERVER| - bind complete\n");
  res = listen(sfd, 5);
  checkRes(&res, "listen error");
  printf("|SERVER| - listening clients\n");

  cfd = accept(sfd, NULL, NULL);
  checkRes(&cfd, "accept error");
  printf("|SERVER| - Accept\n");

  char buff[BUFF_SIZE];

  while (1) {
    res = recv(cfd, buff, BUFF_SIZE, 0);
    checkRes(&res, "recv error");
    printf("|SERVER| - recv complete\n");

    if (strncmp(buff, "/exit", BUFF_SIZE) == 0) {
      close(cfd);
      break;
    }

    strcat(buff, " from server");
    res = send(cfd, buff, BUFF_SIZE, 0);
    checkRes(&res, "send error");
    printf("|SERVER| - echo recv complete\n");
  }

  close(sfd);
  unlink(SOCKET_PATH);
  exit(EXIT_SUCCESS);
}