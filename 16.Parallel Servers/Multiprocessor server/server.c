#include "header.h"

pid_t childMake(int id, int fd, int (*func)(int id, int fd)) {
  pid_t pid;
  pid = fork();
  if (pid == -1) {
    return -1;
  } else if (!pid) {
    exit(func(id, fd));
  } else {
    return pid;
  }
}

int childProcess(int id, int fd) {
  char clientIp[INET_ADDRSTRLEN];
  struct sockaddr_in clientAddr;
  socklen_t len = 0;
  int res = 0, clientFd = 0;
  char recvBuff[BUFF_SIZE], sendBuff[BUFF_SIZE];

  len = sizeof(clientAddr);

  memset(&clientAddr, 0, sizeof(struct sockaddr_in));
  memset(clientIp, 0, sizeof(clientIp));
  memset(recvBuff, 0, sizeof(recvBuff));
  memset(sendBuff, 0, sizeof(sendBuff));

  clientFd = accept(fd, (struct sockaddr *)&clientAddr, &len);
  checkRes(&clientFd, "accept error");

  inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, INET_ADDRSTRLEN);
  printf("------------|SERVER| - Accept-----------------\n");
  printf("Process[%d][PID: %d] number has started\n", id, (int)getpid());
  printf("client ip = %s, port = %d\n", clientIp, ntohs(clientAddr.sin_port));
  printf("----------------------------------------------\n");

  while (1) {
    res = recv(clientFd, recvBuff, sizeof(recvBuff), 0);
    checkRes(&res, "recv error");

    strncat(sendBuff, recvBuff, strlen(recvBuff));
    strncat(sendBuff, " - message received", 20);

    res = send(clientFd, sendBuff, sizeof(sendBuff), 0);
    checkRes(&res, "send error");

    printf("---------------|CLIENT[%d]|-------------------\n", id);
    printf("recv complete : %s\n", recvBuff);
    printf("send complete : %s\n", sendBuff);

    if (!strncmp(recvBuff, BREAKER_CLIENT, strlen(recvBuff) - 1)) {
      printf("|CLIENT[%d]| - Good Bye!\n", id);
      break;
    }

    memset(recvBuff, 0, sizeof(recvBuff));
    memset(sendBuff, 0, sizeof(sendBuff));
  }

  close(clientFd);
  return EXIT_SUCCESS;
}

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