#include "header.h"

int main() {
  int sfd = 0, res = 0, breaker = 1;
  char writeBuff[BUFF_SIZE];
  char readBuff[BUFF_SIZE];

  struct sockaddr_in servAddr;

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  checkRes(&sfd, "socket error");
  printf("|CLIENT| - socket ready\n");

  memset(&servAddr, 0, sizeof(struct sockaddr_in));
  memset(writeBuff, 0, BUFF_SIZE);
  memset(readBuff, 0, BUFF_SIZE);

  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(SIN_PORT);

  res = connect(sfd, (const struct sockaddr *)&servAddr,
                sizeof(struct sockaddr_un));
  checkRes(&res, "connect error");
  printf("|CLIENT| - connect complete\n");

  while (breaker) {
    printf("|CLIENT| - write massage: ");
    fgets(writeBuff, BUFF_SIZE, stdin);

    if (!strncmp(writeBuff, "!exit", strlen(writeBuff) - 1)) {
      res = write(sfd, writeBuff, strlen(writeBuff) - 1);
      checkRes(&res, "write error");
      printf("Good Bye!\n");

      breaker = 0;
    }

    res = write(sfd, writeBuff, strlen(writeBuff) - 1);
    checkRes(&res, "write error");
    printf("|CLIENT| - write complete\n");

    res = read(sfd, readBuff, BUFF_SIZE);
    checkRes(&res, "read error");
    printf("|CLIENT| - read complete\n");

    printf("|CLIENT| - msg sent: %s\n", writeBuff);
    printf("|CLIENT| - echo msg received: %s\n", readBuff);

    memset(writeBuff, 0, BUFF_SIZE);
    memset(readBuff, 0, BUFF_SIZE);
  }

  close(sfd);
  exit(EXIT_FAILURE);
}