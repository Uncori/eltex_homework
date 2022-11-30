#include "fifo.h"

int main(void)
{
    int serverFd = 0, dummyFd = 0, clientFd = 0;
    char clientFifo[CLIENT_FIFO_NAME_LEN];
    request_t req;
    response_t resp;
    req.lenSend = 0;
    resp.lenReceive = 0;
    memset(req.send, '\0', PIPE_BUF);
    memset(resp.receive, '\0', PIPE_BUF);
    int res = 0;
    umask(0);
    printf("Welcome server!\n");
    res = mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP);
    if (res == -1 && errno == EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    printf("mkfifo SERVER_FIFO complete\n");

    serverFd = open(SERVER_FIFO, O_RDONLY);
    if (serverFd == -1) {
        perror("open read");
        exit(EXIT_FAILURE);
    }
    printf("open SERVER_FIFO O_RDONLY complete\n");

    dummyFd = open(SERVER_FIFO, O_WRONLY);
    if (dummyFd == -1) {
        perror("open write");
        exit(EXIT_FAILURE);
    }
    printf("open SERVER_FIFO O_WRONLY complete\n");

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("signal = SIG_ERR");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        printf("Wait read massage.....\n");
        if (read(serverFd, &req, sizeof(request_t)) != sizeof(request_t)) {
            fprintf(stderr, "Error reading request; discarding\n");
            continue;
        }
        printf("req.pid = %d\n", req.pid);
        printf("req.send = %s\n", req.send);
        printf("req.lenSend = %d\n", req.lenSend);
        printf("read serverFd comlete!\n");

        snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,
                 (long)req.pid);
        printf("Added pid\n");

        clientFd = open(clientFifo, O_WRONLY);
        if (clientFd == -1) {
            perror("open clientFifo");
            continue;
        }
        printf("open clientFd comlete!\n");

        printf("Wait write massage.....\n");
        strncat(resp.receive, req.send, strlen(req.send) - 1);
        strncat(resp.receive, " - delivered!", 14);
        resp.lenReceive = strlen(resp.receive);
        if (write(clientFd, &resp, sizeof(response_t)) != sizeof(response_t)) {
            fprintf(stderr, "Error reading request; discarding\n");
        }
        printf("write clientFd comlete!\n");

        if (close(clientFd) == -1) {
            perror("close clientFd");
        }
        printf("close clientFd!\n\n");
        memset(req.send, '\0', PIPE_BUF);
        memset(resp.receive, '\0', PIPE_BUF);
    }
}