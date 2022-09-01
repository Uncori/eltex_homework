#include "header.h"

static void *threadFunc(void *fd);

int main() {
  int socketFd = 0, res = 0, *clientFd = 0;
  char clientIp[INET_ADDRSTRLEN];

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

  while (1) {
    pthread_t tid;

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
    printf("\n");

    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, INET_ADDRSTRLEN);
    printf("------------|SERVER| - Accept-----------------\n");
    printf("client ip = %s,port = %d\n", clientIp, ntohs(clientAddr.sin_port));
    printf("----------------------------------------------\n");

    pthread_create(&tid, NULL, &threadFunc, clientFd);
  }

  close(socketFd);
  exit(EXIT_SUCCESS);
}

static void *threadFunc(void *fd) {
  int conectFd;

  conectFd = *((int *)fd);

  pthread_detach(pthread_self());

  int offProc = 1;
  while (offProc) {
    offProc = processWork(conectFd);
  }

  free(fd);
  close(conectFd);
  pthread_exit(NULL);
}