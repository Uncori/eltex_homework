

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
  long len = 0;

  char *msgBuffer;
  struct mq_attr attr;

  flags = O_WRONLY;

  mqd = mq_open(NAME, flags);

  if (mqd) {
    printf("Server[%d] create \"%s\"\n",mqd, NAME);
  }

  if (!mq_getattr(mqd, &attr)) {
    printf("mq_maxmsg = %ld, mq_msgsize = %ld, mq_curmsgs = %ld\n",
           attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
  }
  
  len = attr.mq_msgsize;

  while(1){
  msgBuffer = calloc(len, sizeof(char));
  fgets(msgBuffer,len,stdin);
  if (!mq_send(mqd, msgBuffer, strlen(msgBuffer), 0)) {
    printf("Message sent = \"%s\"\n", msgBuffer);
    free(msgBuffer);
  } else {
    printf("Message not sent! = \"%s\"\n", msgBuffer);
    free(msgBuffer);
  }
  }


  

 
  if (!mq_close(mqd)) {
    printf("CLOSE!\n");
  }

  free(msgBuffer);
  exit(0);
}
