#include "header.h"

int main() {
  int socketFd = 0, res = 0;
  pid_t child_pid[COUNT_PROCESS];

  struct sockaddr_in servAddr;

  socketFd = socket(AF_INET, SOCK_STREAM, 0);
  checkRes(&socketFd, "socket error");
  printf("|SERVER| - socket ready\n");

  memset(&servAddr, 0, sizeof(struct sockaddr_in));

  servAddr.sin_family = AF_INET;                 // INET TCP
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);  // 127.0.0.1
  servAddr.sin_port = htons(SIN_PORT);           // 5005

  res = bind(socketFd, (const struct sockaddr *)&servAddr,
             sizeof(struct sockaddr_in));
  checkRes(&res, "bild error");
  printf("|SERVER| - bind complete\n");

  res = listen(socketFd, 5);
  checkRes(&res, "listen error");
  printf("|SERVER| - listening clients\n\n");

  for (int i = 0; i < COUNT_PROCESS; ++i) {
    child_pid[i] = childMake(i, socketFd, childProcess);
    printf("SERVER childPid[%d] = %d\n", i, (int)child_pid[i]);
  }

  while (1) {
  }
  close(socketFd);
  exit(EXIT_SUCCESS);
}