#include "header.h"

void checkRes(const int *res, const char *msg) {
  if (*res == ERROR_RETURN) {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}

void sigChild(int sign) {
  pid_t pid;
  int status;
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    printf("|SERVER| - child %d stopped or terminated with sing %d\n", pid,
           sign);
  }
}

void processWork(int clientFd) {
  int res = 0;
  char buff[BUFF_SIZE];
  memset(buff, 0, sizeof(buff));
  printf("|SERVER| - child process ready %d parent %d\n", getpid(), getppid());

  res = recv(clientFd, buff, sizeof(buff), 0);
  checkRes(&res, "recv error");
  printf("|SERVER| - recv complete : %s\n", buff);

  strncat(buff, " - message received", 20);

  res = send(clientFd, buff, sizeof(buff), 0);
  checkRes(&res, "send error");
  printf("|SERVER| - echo recv complete : %s\n\n", buff);

  memset(buff, 0, sizeof(buff));
}