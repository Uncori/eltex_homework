#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SERV_NAME "/server"

void checkRes(const int *res) {
  if (*res == -1) exit(EXIT_FAILURE);
}

int main() {
  int flags = 0, res = 0;
  int countClient = 0, offServ = 1;
  mqd_t mqd;

  unsigned int prio;
  void *buffer;
  ssize_t numRead;

  struct mq_attr attr;

  flags = O_RDONLY | O_CREAT;

  mqd = mq_open(SERV_NAME, flags, 0777, NULL);

  if (mqd) {
    printf("Server[%d] start \"%s\"\n", mqd, SERV_NAME);
  }
  res = mq_getattr(mqd, &attr);
  checkRes(&res);
  printf(
      "\tmax count message = %ld\n\tmax messagesize = %ld\n\tcurmsgs = %ld\n",
      attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

  long len = 0;
  len = attr.mq_msgsize;

  while (offServ) {
    buffer = calloc(len, sizeof(char));
    numRead = mq_receive(mqd, buffer, len, &prio);
    if (numRead != -1) {
      if (!strncmp((char *)buffer, "[client connect]\n", 18)) {
        ++countClient;
      }
      if (!strncmp((char *)buffer, "[client disconnect]\n", 21)) {
        --countClient;
      }
      printf("%s", (char *)buffer);
      free(buffer);
    }
    if (!countClient) {
      offServ = 0;
    }
  }

  res = mq_close(mqd);
  checkRes(&res);
  printf("serv [%d] \"%s\" close!\n", mqd, SERV_NAME);

  res = mq_unlink(SERV_NAME);
  checkRes(&res);
  printf("serv [%d] \"%s\" unlink!\n", mqd, SERV_NAME);

  exit(0);
}
