#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define NAME "/server"

struct mq_attr attr;


int main() {
  int flags = 0;
  mqd_t mqd;

  
  attr.mq_curmsgs = 0;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = 255;

  flags = O_RDONLY | O_CREAT;
  mqd = mq_open(NAME, flags, 0777, &attr);
    
  if(mqd){
    printf("CREATE \"%s\"\n", NAME);
  }
  
  if(!mq_getattr(mqd, &attr)){
      printf ("max #msgs = %ld, max #bytes/msg = %ld, "
"#currently on queue = %ld\n",
attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
  }

  if(!mq_send(mqd,"Hello World!\n", 14, 10)){
    printf("SEND!\n");
  }

  if(mq_receive(mqd,"Hello World_t\n", 15, (unsigned int*)10)){
    printf("RECEIVE!\n");
  }

  if(!mq_close(mqd)){
    printf("CLOSE!\n");
  }

  if(!mq_unlink(NAME)) {
    printf("UNLINK!\n");
  }
 
  exit(0);
}
