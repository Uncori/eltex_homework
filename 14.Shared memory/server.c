#include "header.h"

int main() {
    shmdata *data;
    int fd = 0, offServ = 1;
    pid_t pid;
    fd = shm_open(IPCNAME, O_RDWR | O_CREAT, FILEMOD);
    data = mmap(NULL, sizeof(shmdata), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ftruncate(fd, sizeof(shmdata));
    close(fd);
    pid = getpid();
    data->countMsg = 0;
    data->countClient = 0;
    sem_init(&data->mutex, 1, MAXCLIENT + 1);
    while(offServ) {
        if(data->countMsg > 0) {
        sem_wait(&data->mutex);
        printf("msg = %s pid = %d\n", data->mesg, pid);
        --data->countMsg;
        sem_post(&data->mutex);
        }
    if(!data->countClient){
      offServ = 0;
    }
    }
    shm_unlink(IPCNAME);
    exit(EXIT_SUCCESS);
}