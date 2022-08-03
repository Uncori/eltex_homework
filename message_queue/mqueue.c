#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  mqd_t mqd;
  struct mq_attr attr;
  mqd = mq_open(argv[1], O_RDONLY);
  mq_getattr(mqd, &attr);
  printf("Maximum # of messages on queue: %ld\n", attr.mq_maxmsg);
  printf("Maximum message size: %ld\n", attr.mq_msgsize);
  printf("# of messages currently on queue: %ld\n", attr.mq_curmsgs);
  exit(EXIT_SUCCESS);
}