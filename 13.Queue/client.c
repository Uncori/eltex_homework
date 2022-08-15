#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define NAME "/server"

void checkRes(const int *res) {
  if (*res == -1) exit(EXIT_FAILURE);
}

int main() {
  int flags = 0, res = 0;
  mqd_t mqd;
  long len = 0;

  char *msgBuffer;
  char *nicName;
  char *outputMsg;

  struct mq_attr attr;

  flags = O_WRONLY;

  mqd = mq_open(NAME, flags);

  if (mqd) {
    nicName = calloc(255, sizeof(char));
    printf("Write your nickname: ");
    fgets(nicName, 255, stdin);

    res = mq_send(mqd, "[client connect]\n", 18, 0);
    checkRes(&res);

    printf("Hello, %s\tServer[%d] connect \"%s\"\n", nicName, mqd, NAME);
    res = mq_getattr(mqd, &attr);

    checkRes(&res);
    printf("\tmq_maxmsg = %ld\tmq_msgsize = %ld\tmq_curmsgs = %ld\n",
           attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

    printf("\tto exit print 'exit'\n");

    len = attr.mq_msgsize;

    while (1) {
      msgBuffer = calloc(255, sizeof(char));
      outputMsg = calloc(len, sizeof(char));
      fgets(msgBuffer, len, stdin);
      strncat(outputMsg, nicName, strlen(nicName) - 1);
      strncat(outputMsg, ": ", 3);
      strncat(outputMsg, msgBuffer, strlen(msgBuffer));

      if (!strncmp(msgBuffer, "exit\n", 6)) {
        free(msgBuffer);
        msgBuffer = calloc(255, sizeof(char));
        strncat(msgBuffer, "[client disconnect]\n", 21);
        mq_send(mqd, msgBuffer, strlen(msgBuffer), 0);
        break;
      }
      if (!mq_send(mqd, outputMsg, strlen(outputMsg), 0)) {
        printf("[message sent]\n");
        free(outputMsg);
        free(msgBuffer);
      } else {
        printf("[message not sent!]\n");
        free(outputMsg);
        free(msgBuffer);
      }
    }

    res = mq_close(mqd);
    checkRes(&res);
    printf("Good bye!\n");

    free(outputMsg);
    free(nicName);
    free(msgBuffer);
  } else {
    printf("No connection to server!\n");
  }
  exit(0);
}
