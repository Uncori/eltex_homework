#include "fifo.h"

static char clientFifo[CLIENT_FIFO_NAME_LEN];

static void removeFifo(void) { unlink(clientFifo); }

int main(void)
{
    int serverFd = 0, clientFd = 0, res = 0;
    request_t req;
    response_t resp;
    req.lenSend = 0;
    resp.lenReceive = 0;

    memset(req.send, '\0', PIPE_BUF);
    memset(resp.receive, '\0', PIPE_BUF);

    umask(0);
    snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,
             (long)getpid());
    printf("pid added!\n");
    res = mkfifo(clientFifo, S_IRUSR | S_IWUSR | S_IWGRP);
    if (res == -1 && errno == EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    printf("mkfifo clientFifo = %s complete\n", clientFifo);

    res = atexit(removeFifo);
    if (res != 0) {
        perror("attexit");
        exit(EXIT_FAILURE);
    }
    printf("Write your massage:\n");
    fgets(req.send, PIPE_BUF, stdin);
    req.pid = getpid();
    req.lenSend = strlen(req.send);

    serverFd = open(SERVER_FIFO, O_WRONLY);
    if (serverFd == -1) {
        perror("open serverFd");
        exit(EXIT_FAILURE);
    }
    printf("open SERVER_FIFO O_WRONLY complete\n");

    if (write(serverFd, &req, sizeof(request_t)) != sizeof(request_t)) {
        perror("write serverFd");
        exit(EXIT_FAILURE);
    }
    printf("write serverFd comlete!\n");

    clientFd = open(clientFifo, O_RDONLY);
    if (clientFd == -1) {
        perror("open clientFifo");
        exit(EXIT_FAILURE);
    }
    printf("open SERVER_FIFO O_RDONLY complete\n");

    if (read(clientFd, &resp, sizeof(response_t)) != sizeof(response_t)) {
        perror("read clietnFd");
        exit(EXIT_FAILURE);
    }
    printf("read clientFd comlete!\n");
    printf("resp.receive = %s\n", resp.receive);
    printf("resp.lenReceive = %d\n", resp.lenReceive);

    unlink(clientFifo);

    exit(EXIT_SUCCESS);
}