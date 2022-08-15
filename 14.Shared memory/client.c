#include "header.h"

int main() {
    shmdata *data;
    int fd = 0, offClient = 1;
    char outputMsg[MESGSIZE];
    char nickName[NICKNAMESIZE];
    char mesg[MESGSIZE - NICKNAMESIZE];

    fd = shm_open(IPCNAME, O_RDWR, FILEMOD);
    data = mmap(NULL, sizeof(shmdata), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    printf("Enter your nick: ");
    fgets(nickName, NICKNAMESIZE, stdin);
    
    ++data->countClient;

    while(offClient) {

    printf("Enter your message: ");
    fgets(mesg, MESGSIZE - NICKNAMESIZE, stdin);

    if(!strncmp(mesg,"exit\n",6)){
        --offClient;
        strncat(outputMsg, nickName, strlen(nickName) - 1);
        strncat(outputMsg, " disconnect\n", 13);

        sem_wait(&data->mutex);
        memset(data->mesg, '\0', MESGSIZE);
        strncpy(data->mesg,outputMsg, strlen(outputMsg));
        ++data->countMsg;
        --data->countClient;
        sem_post(&data->mutex);
        break;
    }
    strncat(outputMsg, nickName, strlen(nickName) - 1);
    strncat(outputMsg, " : ", 4);
    strncat(outputMsg, mesg, strlen(mesg));

    sem_wait(&data->mutex);
    memset(data->mesg, '\0', MESGSIZE);
    strncpy(data->mesg,outputMsg, strlen(outputMsg));
    ++data->countMsg;
    sem_post(&data->mutex);
    memset(outputMsg, '\0', MESGSIZE);
    }

    exit(EXIT_SUCCESS);
}