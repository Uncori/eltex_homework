#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define NAME "/server"

int main() {
  int flags = 0, error = 0;
  mqd_t mqd;

  unsigned int prio;
  char *buffer;
  ssize_t numRead;

  buffer = "Hello World!";

  struct mq_attr attr;
  long len = 0;
  len = attr.mq_msgsize;

  //buffer = calloc(len, sizeof(char));

  flags = O_WRONLY | O_CREAT;
  mqd = mq_open(NAME, flags, 0777, NULL);

  if (mqd) {
    printf("Server[%d] create \"%s\"\n",mqd, NAME);
  }

  if (!mq_send(mqd, buffer, strlen(buffer), 0)) {
    printf("Message sent = \"%s\"\n", buffer);
  }

  if (!mq_getattr(mqd, &attr)) {
    printf("mq_maxmsg = %ld, mq_msgsize = %ld, mq_curmsgs = %ld\n",
           attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
  }

  if (!mq_close(mqd)) {
    printf("CLOSE!\n");
  }

  /*if(!mq_unlink(NAME)) {
    printf("UNLINK!\n");
  }*/

  exit(0);
}
