#include "header.h"

int main() {
    shmdata *data;
    int fd = 0, offServ = 1;
    fd = shm_open(IPCNAME, O_RDWR | O_CREAT, FILEMOD);
    data = mmap(NULL, sizeof(shmdata), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ftruncate(fd, sizeof(shmdata));
    close(fd);
    data->countMsg = 0;
    data->countClient = 0;
    sem_init(&data->mutex, 1, MAXCLIENT + 1);

    while(offServ) {
        if(data->countMsg > 0) {
        sem_wait(&data->mutex);
        printf("%s", data->mesg);
        --data->countMsg;
         if(!data->countClient){
      --offServ;
    }
        sem_post(&data->mutex);
        }
        

    }
    printf("server close!\n");
    shm_unlink(IPCNAME);
    exit(EXIT_SUCCESS);
}