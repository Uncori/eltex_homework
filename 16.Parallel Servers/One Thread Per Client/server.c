#include "header.h"

static void *threadFunc(void *fd);

int main() {
  int socketFd = 0, res = 0, *clientFd = 0, i = 0;
  pthread_t tid;

  char clientIp[INET_ADDRSTRLEN];
  sockInfo info[COUNT_CLIENT];

  socklen_t len = 0;
  struct sockaddr_in servAddr, clientAddr;

  socketFd = socket(AF_INET, SOCK_STREAM, 0);
  checkRes(&socketFd, "socket error");
  printf("|SERVER| - socket ready\n");

  memset(&servAddr, 0, sizeof(struct sockaddr_in));
  memset(clientIp, 0, sizeof(clientIp));

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

  while (i < COUNT_CLIENT) {
    len = sizeof(clientAddr);
    clientFd = malloc(sizeof(int));

    *clientFd = accept(socketFd, (struct sockaddr *)&clientAddr, &len);
    if (*clientFd < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        printf("|SEVER| - accept error!\n");
        break;
      }
    }
    info[i].fd = *clientFd;
    info[i].addr = clientAddr;
    info[i].id = tid;

    printf("\n");

    inet_ntop(AF_INET, &info[i].addr.sin_addr, clientIp, INET_ADDRSTRLEN);
    printf("------------|SERVER| - Accept-----------------\n");
    printf("client id = [%d] ip = %s, port = %d\n", i + 1, clientIp,
           ntohs(info[i].addr.sin_port));
    printf("----------------------------------------------\n");
    memset(clientIp, 0, INET_ADDRSTRLEN);

    pthread_create(&tid, NULL, &threadFunc, clientFd);
    ++i;
  }

  close(socketFd);
  exit(EXIT_SUCCESS);
}

static void *threadFunc(void *fd) {
  int conectFd = 0;

  conectFd = *((int *)fd);

  pthread_detach(pthread_self());

  int offProc = 1;
  while (offProc) {
    int res = 0;
    char buff[BUFF_SIZE];
    memset(buff, 0, sizeof(buff));

    res = recv(conectFd, buff, sizeof(buff), 0);
    checkRes(&res, "recv error");
    printf("|SERVER| - recv complete : %s\n", buff);

    if (!strncmp(buff, "!exit\n", 7)) {
      offProc = 0;
    }

    strncat(buff, " - message received", 20);

    res = send(conectFd, buff, sizeof(buff), 0);
    checkRes(&res, "send error");
    printf("|SERVER| - echo recv complete : %s\n\n", buff);

    memset(buff, 0, BUFF_SIZE);
  }

  free(fd);
  close(conectFd);
  pthread_exit(NULL);
}