#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <errno.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "rbtree.h"

#define N 30
#define PING_PKT_DATA_SZ 300
#define SERV_BUF_SIZE 500
#define RECV_FLAGS MSG_WAITALL
#define TET_LEN 7
#define _err_ok 0
#define _err_failed -1
#define _err_timeout 1
#define MAX_PLAYERS_NUM 7

//время ожидания игроков
#define WAIT_USEC 5
#define RESP_WAIT_USEC 2000
#define DEBUG 0
#define FALL_TIME_DEC 10
#define FALL_TIME_MIN 300

typedef struct {
    struct icmp hdr;
    char data[PING_PKT_DATA_SZ];
} Ping_pkt_t;

typedef struct {
    struct ip ip_hdr;
    Ping_pkt_t ping_pkt;
} Ip_pkt_t;

typedef struct {
    int cur;
    int next;
} Figure_info;

typedef struct {
    Figure fig;
    int fig_size;
    int score;
} Figure_send;

typedef struct {
    int next_fig_id;
    int score;
} Cup_send;

enum role { r_serv = 1, r_client = 2 };

enum operation {
    st_get_ip = 0,
    st_lobby_connect = 1,
    st_answer = 2,
    st_check = 3,
    st_check_answer = 4,
    st_serv_move = 5,
    st_key = 6,
    st_serv_down = 7,
    st_serv_quit = 8
};

int get_num_of_players();
ushort checksum(void *b, int len);
void prepare_icmp_pkt(Ping_pkt_t *ping_pkt, u_int16_t option, char *data,
                      int data_size);
void *serv_thread();
ulong get_cur_time_ms();
int sendto_icmp(int sock, struct sockaddr_in serv, int option, char *data,
                int data_size);
int recvfrom_time(int sock, struct sockaddr_in serv, char *buf,
                  ulong start_time, ulong wait_time);
int broadcast_alpha(RBT *rbt, int num_of_players, int option, char *data,
                    int data_size);
Node *find_by_ip_btree(RBT *btr, struct in_addr addr);
Node *find_by_id_btree(RBT *btr, int id);
int recvfrom_all_time_alpha(RBT *rbt, int num_of_players, int sock_client,
                            ulong start_time, ulong wait_time);
int icmp_check(Ip_pkt_t *pkt);
int get_rand_element(int *mass, int size);
int serv_game(int sock_in, RBT *rbt, int num_of_players, int cur_figure_id,
              int next_figure_id);
int send_move(RBT *rbt, int num_of_players, int sock, Figure figure_serv,
              char *send_buf);
int send_cup(RBT *rbt, int num_of_players, int sock, char *send_buf,
             int next_figure_id);
int cup_copy(int **cup, char *send_buf);

#endif // SERVER_H
