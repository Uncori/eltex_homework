#include "header.h"

void checkRes(const int *res, const char *msg) {
  if (*res == ERROR_RETURN) {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}

int processWork(int clientFd) {
  int res = 0, offProc = 1;
  char buff[BUFF_SIZE];
  memset(buff, 0, sizeof(buff));
  printf("|SERVER| - child process ready %d parent %d\n", getpid(), getppid());

  res = recv(clientFd, buff, sizeof(buff), 0);
  checkRes(&res, "recv error");
  printf("|SERVER| - recv complete : %s\n", buff);

  if (!strncmp(buff, "!exit\n", 7)) {
    offProc = 0;
  }

  strncat(buff, " - message received", 20);

  res = send(clientFd, buff, sizeof(buff), 0);
  checkRes(&res, "send error");
  printf("|SERVER| - echo recv complete : %s\n\n", buff);

  return offProc;
}