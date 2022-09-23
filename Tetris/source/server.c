#include <arpa/inet.h>
#include <curses.h>
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

#include "../header/cup.h"
#include "../header/handler.h"
#include "../header/inet.h"
#include "../header/interface.h"
#include "../header/server.h"

extern int start_game;
extern int ip_written;
extern int serv_cli_fig_id;
int num_of_pl = 0;
int empty;

int get_num_of_players() { return num_of_pl; }

ushort checksum(void *b, int len)
{
    ushort *buf = b;
    uint sum = 0;
    ushort result;

    for (sum = 0; len > 1; len -= 2) {
        sum += *buf++;
    }
    if (len == 1) {
        sum += *(unsigned char *)buf;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void prepare_icmp_pkt(Ping_pkt_t *ping_pkt, u_int16_t option, char *data,
                      int data_size)
{
    memset(ping_pkt, 0, sizeof(Ping_pkt_t));
    strcpy(ping_pkt->data, "tetris|");
    if (data_size == 0) {
        ping_pkt->data[TET_LEN] = (char)(strlen(data) + 1);
        strcpy(&(ping_pkt->data[TET_LEN + 1]), data);
    } else {
        for (int i = 0; i < data_size; ++i) {
            ping_pkt->data[TET_LEN + 1 + i] = data[i];
        }
    }
    ping_pkt->data[PING_PKT_DATA_SZ - 1] = '\0';

    ping_pkt->hdr.icmp_type = ICMP_ECHO;
    ping_pkt->hdr.icmp_hun.ih_idseq.icd_id = option;
    ping_pkt->hdr.icmp_hun.ih_idseq.icd_seq = 1;
    ping_pkt->hdr.icmp_cksum = checksum(ping_pkt, sizeof(Ping_pkt_t));
}

void *serv_thread()
{
    struct sockaddr_in serv;
    struct sockaddr_in client;
    char buf_recv_info[SERV_BUF_SIZE];

    int i;
    uint size_buf_recv;
    int get_len;
    int stat_return_func;
    int ret;
    int timeout = 0;

    Ping_pkt_t pkt;
    Player player;

    struct pollfd fd_in;

    const int sock_in = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock_in < 0) {
        perror("socket(): in\n");
        pthread_exit(NULL);
    }
    const int sock_out = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock_out < 0) {
        perror("socket(): out\n");
        pthread_exit(NULL);
    }
    srand(time(NULL));
    stat_return_func =
        inet_aton("8.8.8.8", (struct in_addr *)&serv.sin_addr.s_addr);
    serv.sin_family = AF_INET;
    if (!stat_return_func) {
        perror("inet_aton(): ");
        close(sock_in);
        close(sock_out);
        pthread_exit(NULL);
    }

    stat_return_func = sendto_icmp(sock_out, serv, st_get_ip, "get_ip", 0);
    if (stat_return_func <= 0) {
        perror("sendto(): ");
        close(sock_in);
        close(sock_out);
        pthread_exit(NULL);
    }

    size_buf_recv = sizeof(client);

    while (1) {
        memset(&client, 0, sizeof(client));
        get_len = recvfrom(sock_in, buf_recv_info, SERV_BUF_SIZE, RECV_FLAGS,
                           (struct sockaddr *)&client, &size_buf_recv);
        if (get_len <= 0) {
            perror("recvfrom(): ");
            pthread_exit(NULL);
        }
        if (strcmp(
                "get_ip",
                &(((Ip_pkt_t *)buf_recv_info)->ping_pkt.data[TET_LEN + 1])) ==
            0) {
            break;
        }
    }

    char *serv_ip = get_server_ip();
    strncpy(serv_ip, inet_ntoa(((Ip_pkt_t *)buf_recv_info)->ip_hdr.ip_dst), N);
    ip_written = 1;
    fprintf(stderr, "Server IP(server): %s\n", serv_ip);

    fd_in.fd = sock_in;
    fd_in.events = POLLIN;
    Node *player_ptr;

    int *pool = (int *)malloc(COUNT_FIGURE * sizeof(int));
    if (pool == NULL) {
        close(sock_in);
        close(sock_out);
        pthread_exit(NULL);
    }
    for (i = 0; i < COUNT_FIGURE; i++) {
        pool[i] = i + 1;
    }
    int pool_size = COUNT_FIGURE;

    player.p_id = num_of_pl + 1;
    inet_aton(serv_ip, (struct in_addr *)&(player.p_ip_addr.s_addr));
    player.p_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (player.p_sock < 0) {
        perror("player sock fail: ");
    }

    player.p_debt = 0;
    player.p_figure = get_rand_element(pool, pool_size);
    pool_size--;
    ++num_of_pl;

    serv_cli_fig_id = player.p_figure;

    add_player_in_figure(player, player.p_figure);
    fprintf(stderr, "\tp[0]\np_id = %d\np_figure = %d\np_ip = %s\n",
            player.p_id, player.p_figure,
            inet_ntoa(*((struct in_addr *)&(player.p_ip_addr.s_addr))));
    player_ptr = find_by_id_btree(get_rbt(), 0);
    fprintf(
        stderr, "\tp[0]\np_id = %d\np_figure = %d\np_ip = %s\n",
        player_ptr->player.p_id, player_ptr->player.p_figure,
        inet_ntoa(*((struct in_addr *)&(player_ptr->player.p_ip_addr.s_addr))));

    fprintf(stderr, "\t|Poll test|\n");
    while (1) {
        memset(&client, 0, sizeof(client));

        ret = poll(&fd_in, 1, WAIT_USEC);
        switch (ret) {
        case -1:
            perror("poll(): ");
            close(sock_in);
            close(sock_out);
            for (i = 0; i < num_of_pl; ++i) {
                player_ptr = rbtree_find_player(get_rbt(), i + 1);
                close(player_ptr->player.p_sock);
            }
            pthread_exit(NULL);
        case 0:
            timeout = 1;
            break;
        default:
            if (fd_in.revents & POLLIN) {
                fd_in.revents = 0;
            }
            break;
        }

        if (timeout) {
            timeout = 0;
            if (start_game) {
                sleep(1);
                break;
            } else {
                timeout = 0;
            }
            continue;
        }

        get_len = recvfrom(sock_in, buf_recv_info, SERV_BUF_SIZE, RECV_FLAGS,
                           (struct sockaddr *)&client, &size_buf_recv);
        if (get_len <= 0) {
            perror("recvfrom(): ");
            close(sock_in);
            close(sock_out);
            for (i = 0; i < num_of_pl; ++i) {
                player_ptr = rbtree_find_player(get_rbt(), i + 1);
                close(player_ptr->player.p_sock);
            }
            pthread_exit(NULL);
        }

        if (icmp_check((Ip_pkt_t *)buf_recv_info) == 0 ||
            (((Ip_pkt_t *)buf_recv_info)
                 ->ping_pkt.hdr.icmp_hun.ih_idseq.icd_id != st_lobby_connect)) {
            continue;
        } else {
            if (find_by_ip_btree(get_rbt(),
                                 ((Ip_pkt_t *)buf_recv_info)->ip_hdr.ip_src) !=
                NULL) {
                fprintf(stderr, "Player already connected\n");
                prepare_icmp_pkt(&pkt, st_answer, "You already in lobby.",
                                 0); // tetris|
                sendto(sock_out, &pkt, sizeof(Ping_pkt_t), 0,
                       (struct sockaddr *)&client, sizeof(struct sockaddr));
            } else {
                player.p_id = num_of_pl + 1;
                player.p_ip_addr.s_addr =
                    ((Ip_pkt_t *)buf_recv_info)->ip_hdr.ip_src.s_addr;
                player.p_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
                if (player.p_sock < 0) {
                    perror("player sock fail: ");
                    perror("socket(): player: ");
                    pthread_exit(NULL);
                }

                player.p_debt = 0;
                player.p_figure = get_rand_element(pool, pool_size);
                pool_size--;
                add_player_in_figure(player, player.p_figure);

                fprintf(stderr, "player[%d] : figure - %d\n", num_of_pl,
                        player.p_figure);

                client.sin_port = 0;
                fprintf(
                    stderr, "Client: %s : connected\n\tMessage: '%s'\n",
                    inet_ntoa(((Ip_pkt_t *)buf_recv_info)->ip_hdr.ip_src),
                    (&((Ip_pkt_t *)buf_recv_info)->ping_pkt.data[TET_LEN + 1]));
                i = sendto_icmp(player.p_sock, client, st_answer,
                                (char *)&(player.p_figure),
                                sizeof(player.p_figure));
                if (i < 0) {
                    perror("send(to)_icmp() in serv_func():");
                } else {
                    fprintf(stderr, "sendto_icmp to player[%d]\n",
                            num_of_pl - 1);
                }
                ++num_of_pl;
            }
            if (num_of_pl >= MAX_PLAYERS_NUM) {
                while (1) {
                    if (start_game) {
                        break;
                    }
                }
            }
        }
    }

    fprintf(stderr, "Poll END\n");
    int cur_figure_id, next_figure_id;
    int num_of_figures_rest = num_of_pl;
    int players_id_mass[num_of_pl];

    for (i = 0; i < num_of_pl; ++i) {
        players_id_mass[i] = i + 1;
    }

    player_ptr = find_by_id_btree(
        get_rbt(), get_rand_element(players_id_mass, num_of_figures_rest));
    cur_figure_id = player_ptr->player.p_figure;
    num_of_figures_rest--;
    if (num_of_figures_rest <= 0) {
        num_of_figures_rest = num_of_pl;
    }
    player_ptr = find_by_id_btree(
        get_rbt(), get_rand_element(players_id_mass, num_of_figures_rest));
    next_figure_id = player_ptr->player.p_figure;
    // num_of_figures_rest--;
    // if (num_of_figures_rest <= 0) {
    //     num_of_figures_rest = num_of_pl;
    // }
    Figure_info fig_info;
    fig_info.cur = cur_figure_id;
    fig_info.next = next_figure_id;
    fprintf(stderr, "\tAll in lobby check: num_of_players: %d\n", num_of_pl);
    ret = broadcast_alpha(get_rbt(), num_of_pl, st_check, (char *)&fig_info,
                          sizeof(fig_info));
    if (ret < 0) {
        fprintf(stderr, "client.c: b_a\n");
    }
    ret = recvfrom_all_time_alpha(get_rbt(), num_of_pl, sock_in,
                                  get_cur_time_ms(), WAIT_USEC);
    if (ret < 0) {
        fprintf(stderr, "client.c: r_a_t_a\n");
    }

    num_of_pl =
        serv_game(sock_in, get_rbt(), num_of_pl, cur_figure_id, next_figure_id);

    num_of_pl = 0;

    pthread_exit(NULL);
}

ulong get_cur_time_ms()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    ulong time_ms = time.tv_sec * 1000 + (time.tv_nsec / 1000000);
    return time_ms;
}

int sendto_icmp(int sock, struct sockaddr_in serv, int option, char *data,
                int data_size)
{
    if (data_size > (PING_PKT_DATA_SZ - (TET_LEN + 1))) {
        return -2;
    }
    int ret;
    Ping_pkt_t pkt;
    prepare_icmp_pkt(&pkt, option, data, data_size);
    ret = sendto(sock, &pkt, sizeof(Ping_pkt_t), 0, (struct sockaddr *)&serv,
                 sizeof(struct sockaddr));

    return ret;
}

int recvfrom_time(int sock, struct sockaddr_in serv, char *buf,
                  ulong start_time, ulong wait_time)
{
    struct pollfd fd_serv;
    int timeout = 0;
    int get_len;
    unsigned int size;

    fd_serv.fd = sock;
    fd_serv.events = POLLIN;
    size = sizeof(serv);

    while (1) {
        int ret = poll(&fd_serv, 1, WAIT_USEC);
        switch (ret) {
        case -1:
            perror("poll(): ");
            return _err_failed;
        case 0:
            timeout = 1;
            break;
        default:
            if (fd_serv.revents & POLLIN) {
                fd_serv.revents = 0;
            }
            break;
        }

        if (timeout) {
            timeout = 0;
            if ((get_cur_time_ms() - start_time) > wait_time) {
                return _err_failed;
            } else {
                timeout = 0;
            }
            continue;
        }
        get_len = recvfrom(sock, buf, SERV_BUF_SIZE, RECV_FLAGS,
                           (struct sockaddr *)&serv, &size);
        break;
    }

    return get_len;
}

int broadcast_alpha(RBT *rbt, int num_of_players, int option, char *data,
                    int data_size)
{
    int i;
    Node *ptr;
    struct sockaddr_in client;
    for (i = 0; i < num_of_players; i++) {
        memset(&client, 0, sizeof(client));
        client.sin_family = AF_INET;
        client.sin_port = 0;

        ptr = find_by_id_btree(rbt, i + 1);
        client.sin_addr.s_addr = ptr->player.p_ip_addr.s_addr;
        if (DEBUG) {
            fprintf(stderr, "server.c: b_a(): send to ip - %s\n",
                    inet_ntoa(client.sin_addr));
        }

        if (sendto_icmp(ptr->player.p_sock, client, option, data, data_size) <=
            0) {
            perror("sendto_icmp() in broadcast_alpha():");
            return _err_failed;
        }
        ptr->player.p_debt = 1;
    }

    return 0;
}

Node *find_by_ip_btree(RBT *btr, struct in_addr addr)
{

    for (int i = 0; i < COUNT_FIGURE; i++) {
        Node *ptr = rbtree_find_figure(btr, i + 1);
        if (ptr != NULL) {
            if (ptr->player.p_ip_addr.s_addr == addr.s_addr) {
                return ptr;
            }
        }
    }

    return NULL;
}

Node *find_by_id_btree(RBT *btr, int id)
{

    for (int i = 0; i < COUNT_FIGURE; i++) {
        Node *ptr = rbtree_find_figure(btr, i + 1);
        if (ptr != NULL) {
            if (ptr->player.p_id == id) {
                return ptr;
            }
        }
    }

    return NULL;
}

int recvfrom_all_time_alpha(RBT *rbt, int num_of_players, int sock_client,
                            ulong start_time, ulong wait_time)
{
    if (num_of_players <= 0) {
        fprintf(stderr, "ERR: in recvfrom_all_time_alpha(): No players\n");
        return _err_failed;
    }
    struct pollfd fd_in;
    int get_len, i;

    char buf[SERV_BUF_SIZE];
    struct sockaddr_in client;
    uint size = sizeof(client);

    fd_in.fd = sock_client;
    fd_in.events = POLLIN;
    Node *ptr;

    while (1) {
        memset(&client, 0, sizeof(client));
        int again_flag = 0;
        int ret = poll(&fd_in, 1, WAIT_USEC);
        switch (ret) {
        case -1:
            perror("poll(): ");
            return _err_failed;
        default:
            if (fd_in.revents & POLLIN) {
                fd_in.revents = 0;
                get_len = recvfrom(fd_in.fd, buf, SERV_BUF_SIZE, RECV_FLAGS,
                                   (struct sockaddr *)&client, &size);
                if (get_len <= 0) {
                    fprintf(stderr, "ERR: recvform() in "
                                    "recvfrom_all_time_alpha(): No players\n");
                    return _err_failed;
                }

                if (icmp_check((Ip_pkt_t *)buf) &&
                    (((Ip_pkt_t *)buf)->ping_pkt.hdr.icmp_hun.ih_idseq.icd_id ==
                     st_check_answer)) {
                    ptr =
                        find_by_ip_btree(rbt, ((Ip_pkt_t *)buf)->ip_hdr.ip_src);
                    if (ptr != NULL) {
                        ptr->player.p_debt = 0;
                    } else {
                        fprintf(stderr,
                                "ERR: find_by_ip in recvfrom_all_time_alpha(): "
                                "ip - %s not found\n",
                                inet_ntoa(client.sin_addr));
                    }
                } else {
                    empty = 0;
                }
            }
            break;
        }

        for (i = 0; i < num_of_players; ++i) {
            ptr = find_by_id_btree(rbt, i + 1);
            if (ptr->player.p_debt > 0) {
                again_flag = 1;
                break;
            }
        }

        if ((get_cur_time_ms() - start_time) > wait_time) {
            fprintf(stderr, "Somebody not response (((\n");
            return -1;
        }

        if (again_flag) {
            continue;
        }
        break;
    }

    return 0;
}

int icmp_check(Ip_pkt_t *pkt)
{
    if (pkt == NULL) {
        fprintf(stderr, "Err: icmp_check(): bad pointer\n");
        return 0;
    }

    if ((strncmp("tetris|", pkt->ping_pkt.data, TET_LEN) == 0) &&
        pkt->ping_pkt.hdr.icmp_type == 8) { // Change to const!!!
        return 1;
    }

    return 0;
}

int get_rand_element(int *mass, int size)
{
    if (mass == NULL) {
        fprintf(stderr, "get_rand_element(): mass == NULL");
        return -1;
    }
    if (size <= 0) {
        fprintf(stderr, "get_rand_element(): size == 0");
        return -1;
    }

    int index = rand() % size, element = mass[index], tmp = mass[size - 1];

    mass[size - 1] = element;
    mass[index] = tmp;

    return element;
}

int serv_game(int sock_in, RBT *rbt, int num_of_players, int cur_figure_id,
              int next_figure_id)
{
    if (rbt == NULL) {
        return -1;
    }

    int down_before = 0;

    int wait_time_ms = 1000;

    Figure figure_serv;

    allocate_figure(&figure_serv, 5);
    copy_by_figure_id(&figure_serv, cur_figure_id);
    shift_left(&figure_serv);

    int num_of_figures_rest = num_of_players;
    int players_id_mass[num_of_players];

    for (int i = 0; i < num_of_players; ++i) {
        players_id_mass[i] = i + 1;
    }

    char buf[SERV_BUF_SIZE];
    char send_buf[SERV_BUF_SIZE];
    struct sockaddr_in client;

    int pass_flag = 1;
    int ret;
    Node *player_ptr;
    ulong start_time;

    for (;;) {
        if (!pass_flag) {
            cur_figure_id = next_figure_id;

            player_ptr = find_by_id_btree(
                get_rbt(),
                get_rand_element(players_id_mass, num_of_figures_rest));
            next_figure_id = player_ptr->player.p_figure;
            num_of_figures_rest--;
            if (num_of_figures_rest <= 0) {
                num_of_figures_rest = num_of_pl;
            }
            if (wait_time_ms > FALL_TIME_MIN) {
                wait_time_ms -= FALL_TIME_DEC;
            }
            ret = send_cup(rbt, num_of_players, sock_in, send_buf,
                           next_figure_id);
            if (ret == -1) {
                fprintf(stderr, "server.c: serv_game(): send_cup(). \n");
                broadcast_alpha(rbt, num_of_players, st_serv_quit, "buy", 0);
                return -1;
            }
        } else {
            pass_flag = 0;
        }

        int figure_id = cur_figure_id;
        copy_by_figure_id(&figure_serv, figure_id);
        shift_left(&figure_serv);
        int key_down = 0;
        start_time = get_cur_time_ms();
        while (!key_down) {
            memset(&client, 0, sizeof(client));
            int ch = 0;
            ret = recvfrom_time(sock_in, client, buf, get_cur_time_ms(), 20);
            if (ret == -1) {
                if ((get_cur_time_ms() - start_time) >
                    (long unsigned int)wait_time_ms) {
                    ch = KEY_BACKSPACE;
                    start_time = get_cur_time_ms();
                }
            } else {
                ch = ERR;
                if (icmp_check((Ip_pkt_t *)buf) &&
                    (((Ip_pkt_t *)buf)->ping_pkt.hdr.icmp_hun.ih_idseq.icd_id ==
                     st_key)) {
                    player_ptr =
                        find_by_ip_btree(rbt, ((Ip_pkt_t *)buf)->ip_hdr.ip_src);
                    if (player_ptr == NULL) {
                        fprintf(stderr,
                                "ERR: client.c: find_by_ip(): no such ip\n");
                    } else {
                        if (player_ptr->player.p_figure == cur_figure_id) {
                            ch = *((int *)&(
                                ((Ip_pkt_t *)buf)->ping_pkt.data[TET_LEN + 1]));
                        }
                    }
                }
            }
            int changed = 1;
            switch (ch) {
            case 122:
                rotation_figure(&figure_serv, CLOCKWISE);
                break;
            case KEY_UP:
                rotation_figure(&figure_serv, COUNTCLOWISE);
                break;
            case KEY_DOWN:
                key_down = move_down(&figure_serv);
                down_score();
                break;
            case KEY_LEFT:
                move_left(&figure_serv);
                break;
            case KEY_RIGHT:
                move_right(&figure_serv);
                break;
            case ' ':
                down_before = down_border_figure(&figure_serv);
                while (!key_down) {
                    key_down = move_down(&figure_serv);
                }
                space_score(down_before, down_border_figure(&figure_serv));
                break;
            case 27:
                key_down = 2;
                break;
            case KEY_BACKSPACE:
                if (DEBUG) {
                    for (int i = 0; i < figure_serv.count_point; ++i) {
                        fprintf(stderr, "%d %d \n", figure_serv.point[i].x,
                                figure_serv.point[i].y);
                    }
                }

                key_down = move_down(&figure_serv);
                break;
            default:
                changed = 0;
                break;
            }
            if (key_down == 0 && changed) {
                ret = send_move(rbt, num_of_players, sock_in, figure_serv,
                                send_buf);
                if (ret == -1) {
                    fprintf(stderr, "ERR: send_move(). \n");
                    broadcast_alpha(rbt, num_of_players, st_serv_quit, "buy",
                                    0);
                    return -1;
                }
            }
        }
        if (key_down == 2) {
            break;
        }

        add_figure(&figure_serv);
        cup_analysis(up_border_figure(&figure_serv),
                     down_border_figure(&figure_serv), 0);
    }
    broadcast_alpha(rbt, num_of_players, st_serv_quit, "buy", 0);
    timeout(-1);
    remove_figure(&figure_serv);

    return num_of_players;
}

int send_move(RBT *rbt, int num_of_players, int sock, Figure figure_serv,
              char *send_buf)
{
    if (rbt == NULL || send_buf == NULL) {
        fprintf(stderr, "send_move(): players or send_buf is NULL\n");
        return -1;
    }

    Figure_send fig_send;
    fig_send.fig = figure_serv;
    fig_send.fig_size = figure_serv.count_point;
    fig_send.score = get_score();
    memcpy(send_buf, &fig_send, sizeof(fig_send));
    memcpy(&(send_buf[sizeof(fig_send)]), fig_send.fig.point,
           fig_send.fig_size * sizeof(Figure_point));
    broadcast_alpha(rbt, num_of_players, st_serv_move, send_buf,
                    sizeof(fig_send) +
                        fig_send.fig_size * sizeof(Figure_point));
    recvfrom_all_time_alpha(rbt, num_of_players, sock, get_cur_time_ms(), 1000);

    for (int i = 0; i < num_of_players; ++i) {
        Node *ptr = find_by_id_btree(rbt, i + 1);
        if (ptr->player.p_debt > 0) {
            return -1;
        }
    }

    return 0;
}

int send_cup(RBT *rbt, int num_of_players, int sock, char *send_buf,
             int next_figure_id)
{
    if (rbt == NULL || send_buf == NULL) {
        fprintf(stderr, "ERR: send_move(): players or send_buf is NULL\n");
        return -1;
    }

    int ret = 0;
    int **cur_cup;
    Cup_send cup_send;
    cup_send.next_fig_id = next_figure_id;
    cup_send.score = get_score();
    memcpy(send_buf, &cup_send, sizeof(cup_send));
    cur_cup = get_cup();
    if (cur_cup == NULL) {
        fprintf(stderr, "ERR: send_move(): cup is NULL\n");
        return -1;
    }

    ret = cup_copy(cur_cup, &(send_buf[sizeof(Cup_send)]));
    if (ret == -1) {
        fprintf(stderr, "ERR: cup_copy():\n");
        return -1;
    }
    ret = broadcast_alpha(rbt, num_of_players, st_serv_down, send_buf,
                          sizeof(Cup_send) + (HEIGHT_CUP * WIDTH_CUP));
    if (ret < 0) {
        perror("br_cast() in send_cup()");
    }
    recvfrom_all_time_alpha(rbt, num_of_players, sock, get_cur_time_ms(), 1000);

    for (int i = 0; i < num_of_players; ++i) {
        Node *ptr = find_by_id_btree(rbt, i + 1);
        if (ptr->player.p_debt > 0) {
            return -1;
        }
    }

    return 0;
}

int cup_copy(int **cup, char *send_buf)
{
    if (cup == NULL || send_buf == NULL) {
        return -1;
    }

    int max_el = 0;
    for (int i = 0; i < HEIGHT_CUP; ++i) {
        for (int j = 0; j < WIDTH_CUP; ++j) {
            send_buf[i * WIDTH_CUP + j] = cup[i][j];
            max_el = max_el > send_buf[i * WIDTH_CUP + j]
                         ? max_el
                         : send_buf[i * WIDTH_CUP + j];
        }
    }
    if (max_el == 0) {
        fprintf(stderr, "cup_copy(): empty cup\n");
    }

    return 0;
}
