#include "header.h"

int main() {
  int socketFd = 0, res = 0;
  socklen_t clientLen = 0;
  pid_t childPid[COUNT_PROCESS];

  struct sockaddr_in servAddr;
  clientLen = sizeof(servAddr);

  memset(&servAddr, 0, sizeof(struct sockaddr_in));
  memset(&childPid, 0, sizeof(pid_t));

  socketFd = socket(AF_INET, SOCK_STREAM, 0);
  checkRes(&socketFd, "socket error");

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(SIN_PORT);

  res = bind(socketFd, (const struct sockaddr *)&servAddr, clientLen);
  checkRes(&res, "bild error");

  res = listen(socketFd, 5);
  checkRes(&res, "listen error");

  for (int i = 0; i < COUNT_PROCESS; ++i) {
    childPid[i] = childMake(i, socketFd, childProcess);
    checkRes(&childPid[i], "fork error");
    printf("child process[%d] pid[%d] ready\n", i, childPid[i]);
  }

  printf("---------|SERVER|---------\n");
  printf("|--> socket succeeded    |\n");
  printf("|--> bind succeeded      |\n");
  printf("|--> listen succeeded    |\n");
  printf("|--> fork[%d] succeeded   |\n", COUNT_PROCESS);
  printf("--------------------------\n");

  signal(SIGCHLD, sigChild);

  while (1) {
    // server work
  }

  close(socketFd);
  exit(EXIT_SUCCESS);
}