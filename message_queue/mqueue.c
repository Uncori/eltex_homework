#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>


int main() {
  mqd_t mqd;
  struct mq_attr attr;
  mqd = mq_open("/tmp/test", O_RDONLY);
  mq_getattr(mqd, &attr);
  printf(
      "max #msgs = %ld, max #bytes/msg = %ld,"
      "#currently on queue = %ld\n",
      attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
  mq_close(mqd);
  exit(0);
}









