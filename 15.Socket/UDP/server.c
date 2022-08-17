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
  socklen_t len;
  char buf[BUFF_SIZE];

  sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
  checkRes(&sfd, "socket error");
  printf("|SERVER| - socket create\n");

  memset(&svaddr, 0, sizeof(struct sockaddr_un));
  svaddr.sun_family = AF_UNIX;
  strncpy(svaddr.sun_path, SOCKET_PATH, sizeof(svaddr.sun_path) - 1);

  res = bind(sfd, (struct sockaddr *)&svaddr, sizeof(struct sockaddr_un));
  checkRes(&res, "bing error");
  printf("|SERVER| - bind complete\n");

  while (1) {
    len = sizeof(struct sockaddr_un);
    numBytes =
        recvfrom(sfd, buf, BUFF_SIZE, 0, (struct sockaddr *)&claddr, &len);
    printf("|SERVER| - recvfrom complete : %s\n", buf);

    if (numBytes == -1) {
      perror("recvfrom error");
      exit(EXIT_FAILURE);
    }
    printf("Server received %ld bytes from %s\n", (long)numBytes,
           claddr.sun_path);

    sendto(sfd, buf, numBytes, 0, (struct sockaddr *)&claddr, len);
    printf("|SERVER| - sendto complete : %s\n", buf);

    if (!strncmp(buf, "!server off", 12)) {
      break;
    }
  }

  close(sfd);
  unlink(SOCKET_PATH);
  exit(EXIT_SUCCESS);
}