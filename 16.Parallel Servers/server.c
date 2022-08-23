#include "header.h"

void checkRes(const int *res, const char *msg) {
  if (*res == ERROR_RETURN) {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}

int main() {
  int socketFd = 0, clientFd = 0, res = 0;
  pid_t pid = 0, childPid = 0;
  char buff[BUFF_SIZE];

  socklen_t len = 0;
  struct sockaddr_in servAddr, clientAddr;

  socketFd = socket(AF_INET, SOCK_STREAM, 0);
  checkRes(&socketFd, "socket error");
  printf("|SERVER| - socket ready\n");

  memset(&servAddr, 0, sizeof(struct sockaddr_in));
  memset(buff, 0, sizeof(buff));

  servAddr.sin_family = AF_INET;                 // INET TCP
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);  // 127.0.0.1
  servAddr.sin_port = htons(SIN_PORT);           // 5005

  res = bind(socketFd, (const struct sockaddr *)&servAddr,
             sizeof(struct sockaddr_in));
  checkRes(&res, "bild error");
  printf("|SERVER| - bind complete\n");

  res = listen(socketFd, 5);
  checkRes(&res, "listen error");
  printf("|SERVER| - listening clients\n");

  while (1) {
    len = sizeof(clientAddr);
    clientFd = accept(socketFd, (struct sockaddr *)&clientAddr, &len);

    checkRes(&clientFd, "accept error");
    printf("|SERVER| - Accept\n\n");

    pid = fork();
    checkRes(&pid, "fork");

    if (!pid) {
      childPid = getpid();
      printf("|SERVER| - child process ready %d parent %d\n", childPid,
             getppid());
      close(socketFd);

      res = recv(clientFd, buff, sizeof(buff), 0);
      checkRes(&res, "recv error");
      printf("|SERVER| - recv complete : %s\n", buff);

      res = send(clientFd, buff, sizeof(buff), 0);
      checkRes(&res, "send error");
      printf("|SERVER| - echo recv complete\n\n");

      memset(buff, 0, sizeof(buff));
      close(clientFd);
      exit(EXIT_SUCCESS);
    }

    close(clientFd);
  }

  close(socketFd);
  exit(EXIT_SUCCESS);
}