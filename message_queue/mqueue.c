#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <sys/stat.h>

int main() {
int flags = O_RDWR | O_CREAT;
mqd_t mqd;
mqd = mq_open("/server_chat", flags, 0777,NULL);
mq_close(mqd);
exit(EXIT_SUCSES);

}
