#include "header.h"

int main() {
  int sfd = 0, res = 0;
  char buff[BUFF_SIZE];

  struct sockaddr_in servAddr;

  while (1) {
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    checkRes(&sfd, "socket error");
    printf("|CLIENT| - socket ready\n");

    memset(&servAddr, 0, sizeof(struct sockaddr_in));
    memset(buff, 0, sizeof(buff));

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(SIN_PORT);

    res = connect(sfd, (const struct sockaddr *)&servAddr,
                  sizeof(struct sockaddr_un));
    checkRes(&res, "connect error");
    printf("|CLIENT| - connect complete\n");

    printf("|CLIENT| - write massage: ");
    fgets(buff, BUFF_SIZE, stdin);

    res = write(sfd, buff, strlen(buff) - 1);
    checkRes(&res, "write error");
    printf("|CLIENT| - write complete\n");

    res = read(sfd, buff, BUFF_SIZE);
    checkRes(&res, "read error");
    printf("|CLIENT| - read complete\n");

    printf("|CLIENT| - msg sent: %s\n", buff);
    printf("|CLIENT| - echo msg received: %s\n", buff);

    if (!strncmp(buff, "!exit", 6)) {
      break;
    }

    sleep(2);
    close(sfd);
  }

  close(sfd);
  exit(EXIT_FAILURE);
}