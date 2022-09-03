#include "header.h"

void checkRes(const int *res, const char *msg) {
  if (*res == ERROR_RETURN) {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}

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
