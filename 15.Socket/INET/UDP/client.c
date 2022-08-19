#include "header.h"

void checkRes(const int *res, const char *msg) {
  if (*res == ERROR_RETURN) {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}

int main() {
  struct sockaddr_in servAddr;
  int sfd = 0;
  ssize_t numBytes;
  char resp[BUFF_SIZE], msg[BUFF_SIZE];

  sfd = socket(AF_INET, SOCK_DGRAM, 0);
  checkRes(&sfd, "socket error");
  printf("|CLIENT| - socket create\n");

  memset(&servAddr, 0, sizeof(struct sockaddr_in));
  memset(resp, 0, sizeof(resp));
  memset(msg, 0, sizeof(msg));

  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(SIN_PORT);
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  printf("write massage: ");
  fgets(msg, sizeof(msg), stdin);
  sendto(sfd, msg, strlen(msg) - 1, 0, (struct sockaddr *)&servAddr,
         sizeof(struct sockaddr_in));
  printf("|CLIENT| - sendto complete : %s\n", msg);

  numBytes = recvfrom(sfd, resp, sizeof(resp), 0, NULL, NULL);
  checkRes((int *)&numBytes, "recvfrom");
  printf("Response: %.*s\n", (int)numBytes, resp);

  close(sfd);
  exit(EXIT_FAILURE);
}