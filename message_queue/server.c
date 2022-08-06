#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define NAME "/server"

int main() {
  int flags = 0;
  mqd_t mqd;

  unsigned int prio;
  void *buffer;
  ssize_t numRead;

  struct mq_attr attr;

  flags = O_RDONLY | O_CREAT;

  mqd = mq_open(NAME, flags, 0777, NULL);

  if (mqd) {
    printf("Server[%d] start \"%s\"\n", mqd, NAME);
  }

  if (!mq_getattr(mqd, &attr)) {
    printf(
        "\tmax count message = %ld\n\tmax messagesize = %ld\n\tcurmsgs = %ld\n",
        attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
  }

  long len = 0;
  len = attr.mq_msgsize;

  while (1) {
    buffer = calloc(len, sizeof(char));

    numRead = mq_receive(mqd, buffer, len, &prio);
    if (numRead != -1) {
      printf("Read %ld bytes; priority = %u\n", (long)numRead, prio);
      printf("%s", (char *)buffer);
      free(buffer);
    }
  }

  if (!mq_close(mqd)) {
    printf("CLOSE!\n");
  }

  if (!mq_unlink(NAME)) {
    printf("UNLINK!\n");
  }
  free(buffer);
  exit(0);
}
