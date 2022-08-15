#include "header.h"

int main() {
    shmdata *data;
    int fd = 0;
    char mesg[MESGSIZE];

    fd = shm_open(IPCNAME, O_RDWR, FILEMOD);

    data = mmap(NULL, sizeof(shmdata), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    while(1) {
    sem_wait(&data->mutex);
    fgets(mesg, 255, stdin);
    strncpy(data->mesg,mesg, strlen(mesg));
    ++data->countMsg;
    sem_post(&data->mutex);
    }
    
    exit(EXIT_SUCCESS);
}