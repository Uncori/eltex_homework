#include "header.h"

void checkRes(const int *res, const char *msg) {
  if (*res == ERROR_RETURN) {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}

int main() {
  struct sockaddr_un svaddr, claddr;
  int sfd = 0, res = 0;
  ssize_t numBytes;
  char resp[BUFF_SIZE], msg[BUFF_SIZE];

  sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
  checkRes(&sfd, "socket error");
  printf("|CLIENT| - socket create\n");

  memset(&claddr, 0, sizeof(struct sockaddr_un));
  claddr.sun_family = AF_UNIX;
  snprintf(claddr.sun_path, sizeof(claddr.sun_path), "/tmp/client_socket.%ld",
           (long)getpid());

  res = bind(sfd, (struct sockaddr *)&claddr, sizeof(struct sockaddr_un));
  checkRes(&res, "bind error");
  printf("|CLIENT| - bind complete\n");

  memset(&svaddr, 0, sizeof(struct sockaddr_un));
  svaddr.sun_family = AF_UNIX;
  strncpy(svaddr.sun_path, SOCKET_PATH, sizeof(svaddr.sun_path) - 1);

  printf("write massage: ");
  fgets(msg, BUFF_SIZE, stdin);
  sendto(sfd, msg, strlen(msg) - 1, 0, (struct sockaddr *)&svaddr,
         sizeof(struct sockaddr_un));
  printf("|CLIENT| - sendto complete : %s\n", msg);

  numBytes = recvfrom(sfd, resp, BUFF_SIZE, 0, NULL, NULL);
  printf("Response: %.*s\n", (int)numBytes, resp);

  remove(claddr.sun_path);
  close(sfd);
  exit(EXIT_FAILURE);
}