#include "header.h"

int main() {
  int servFd = 0, res = 0;
  char sendBuff[BUFF_SIZE];
  char recvBuff[BUFF_SIZE];

  struct sockaddr_in servAddr;

  servFd = socket(AF_INET, SOCK_STREAM, 0);
  checkRes(&servFd, "socket error");

  memset(&servAddr, 0, sizeof(struct sockaddr_in));
  memset(sendBuff, 0, BUFF_SIZE);
  memset(recvBuff, 0, BUFF_SIZE);

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(SIN_PORT);

  res = connect(servFd, (const struct sockaddr *)&servAddr,
                sizeof(struct sockaddr_in));
  checkRes(&res, "connect error");

  printf("--------|SERVER|-------\n");
  printf("|--> socket succeeded |\n");
  printf("|--> connect succeeded|\n");
  printf("|  \"!exit\"    to exit |\n");

  while (1) {
    printf("|enter your message: ");
    fgets(sendBuff, BUFF_SIZE, stdin);

    res = send(servFd, sendBuff, strlen(sendBuff) - 1, 0);
    checkRes(&res, "send error");

    res = recv(servFd, recvBuff, sizeof(recvBuff), 0);
    checkRes(&res, "recv error");

    printf("|send succeeded       |\n");
    printf("|recv succeeded       |\n");
    printf("|message send: %s", sendBuff);
    printf("|message received: %s\n", recvBuff);
    printf("-----------------------\n");

    if (!strncmp(sendBuff, BREAKER_CLIENT, strlen(sendBuff) - 1)) {
      printf("|------Good Bye!------|\n");
      break;
    }

    memset(sendBuff, 0, BUFF_SIZE);
    memset(recvBuff, 0, BUFF_SIZE);
  }

  close(servFd);
  exit(EXIT_FAILURE);
}