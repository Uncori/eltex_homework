#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#include "../header/figure.h"
#include "../header/inet.h"

static int count_players = 0, server_fd = 0;
static char server_ip[30] = {};
static struct sockaddr_in server;

int get_server_fd() { return server_fd; }

int set_server_fd(int value) { return server_fd = value; }

char *get_server_ip() { return server_ip; }

int get_count_players() { return count_players; }

int set_count_players(int value) { return count_players = value; }

int init_server()
{
    memset(&server, 0, sizeof(struct sockaddr_in));

    server_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = 0;
    server.sin_port = 0;

    return 0;
}
