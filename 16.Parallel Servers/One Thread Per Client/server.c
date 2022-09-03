#include "header.h"

static void *threadFunc(void *arg);

int main() {
  int socketFd = 0, res = 0, clientFd = 0, i = 0;
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

  while (1) {
    len = sizeof(clientAddr);
    clientFd = accept(socketFd, (struct sockaddr *)&clientAddr, &len);
    if (clientFd < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        printf("|SEVER| - accept error!\n");
        break;
      }
    }

    if (i == COUNT_CLIENT) {
      i = 0;
    }

    info[i].fd = clientFd;
    info[i].addr = clientAddr;
    info[i].id = i;

    printf("\n");

    inet_ntop(AF_INET, &info[i].addr.sin_addr, clientIp, INET_ADDRSTRLEN);
    printf("------------|SERVER| - Accept-----------------\n");
    printf("client id = [%d] fd = [%d] ip = %s, port = %d\n", i + 1, info[i].fd,
           clientIp, ntohs(info[i].addr.sin_port));
    printf("----------------------------------------------\n\n");
    memset(clientIp, 0, INET_ADDRSTRLEN);

    res = pthread_create(&tid, NULL, &threadFunc, (void *)&info[i]);
    if (res) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
    ++i;
  }

  close(socketFd);
  exit(EXIT_SUCCESS);
}

static void *threadFunc(void *arg) {
  char recvBuff[BUFF_SIZE], sendBuff[BUFF_SIZE];
  int res = 0;

  sockInfo info = *((sockInfo *)arg);

  pthread_detach(pthread_self());

  memset(recvBuff, 0, sizeof(recvBuff));
  memset(sendBuff, 0, sizeof(sendBuff));

  while (1) {
    res = recv(info.fd, recvBuff, sizeof(recvBuff), 0);
    checkRes(&res, "recv error");

    strncat(sendBuff, recvBuff, strlen(recvBuff));
    strncat(sendBuff, " - message received", 20);

    res = send(info.fd, sendBuff, sizeof(sendBuff), 0);
    checkRes(&res, "send error");

    printf("---------------|CLIENT[%d]|-------------------\n", info.id + 1);
    printf("recv complete : %s\n", recvBuff);
    printf("send complete : %s\n", sendBuff);

    if (!strncmp(recvBuff, BREAKER_CLIENT, strlen(recvBuff) - 1)) {
      printf("|CLIENT[%d]| - Good Bye!\n", info.id + 1);
      break;
    }

    memset(recvBuff, 0, sizeof(recvBuff));
    memset(sendBuff, 0, sizeof(sendBuff));
  }

  close(info.fd);
  pthread_exit(NULL);
}