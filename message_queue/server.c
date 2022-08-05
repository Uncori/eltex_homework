#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define NAME "/server"

int main() {
  int flags;
  mqd_t mqd;
  struct mq_attr attr;
  flags = O_RDONLY | O_CREAT;
  mqd = mq_open(NAME, flags, 0660, NULL) == -1;
  if(!mqd){
    printf("CREATE \"%s\"\n", NAME);
  }
  mq_getattr(mqd, &attr);
  printf ("max #msgs = %ld, max #bytes/msg = %ld, "
"#currently on queue = %ld\n",
attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
  if(!mq_close(mqd)){
    printf("CLOSE! \"%s\"\n", NAME);
  }
  if(!mq_unlink(NAME)){
    printf("DELETE! \"%s\"\n", NAME);
  }
  exit(0);
}
