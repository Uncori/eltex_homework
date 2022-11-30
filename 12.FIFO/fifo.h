#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_FIFO "/tmp/server_test"
#define CLIENT_FIFO_TEMPLATE "/tmp/client.%ld"
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 20)
#define PIPE_BUF 255

typedef struct {
    pid_t pid;
    int lenPosled;
    char send[PIPE_BUF];
} request_t;

typedef struct {
    int nachaloPosled;
    char receive[PIPE_BUF];
} response_t;