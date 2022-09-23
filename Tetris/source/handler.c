#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "../header/client.h"
#include "../header/cup.h"
#include "../header/handler.h"
#include "../header/inet.h"
#include "../header/interface.h"
#include "../header/server.h"

int start_game = 0;
int serv_cli_fig_id = -1;
const int rand_num = 10;
static int role;
static Figure fig;
static int key_listen = 0, update_lobby = 0;
static pthread_t tid_listen = 0;
static char *buf = NULL;

extern int ip_written;

int init_listen()
{
    key_listen = 0;
    pthread_create(&tid_listen, NULL, serv_thread, NULL);

    return 0;
}

int client_handler()
{
    int ret = 0;
    Figure next_fig;
    Figure my_fig;

    int sock_serv;
    if (role == r_client) {
        sock_serv = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
        if (sock_serv < 0) {
            perror("socket(): ");
            return _err_failed;
        }
        set_server_fd(sock_serv);
    }
    int sock_out = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock_out < 0) {
        perror("socket(): ");
        return _err_failed;
    }
    buf = malloc(SERV_BUF_SIZE * sizeof(char));
    if (buf == NULL) {
        return -1;
    }

    int my_fig_id, cur_fig_id, next_fig_id;

    ret = init_client(role, get_server_ip(), &my_fig_id, &cur_fig_id,
                      &next_fig_id);
    if (ret == _err_failed) {
        fprintf(stderr, "handler.c: client_handler(): init_client == -1");
        close(sock_out);
        close(sock_serv);
        free(buf);
        return _err_failed;
    }
    fprintf(stderr, "init_client return: %d\n", ret);

    set_change_handler(GAME);

    if (role == r_serv) {
        my_fig_id = serv_cli_fig_id;
    }

    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    inet_aton(get_server_ip(), (struct in_addr *)&serv.sin_addr.s_addr);
    serv.sin_port = 0;

    set_my_figure_id(my_fig_id);
    set_next_figure_id(next_fig_id);

    allocate_figure(&fig, 5);
    allocate_figure(&my_fig, 5);
    allocate_figure(&next_fig, 5);
    copy_by_figure_id(&fig, cur_fig_id);
    copy_by_figure_id(&my_fig, my_fig_id);
    copy_by_figure_id(&next_fig, next_fig_id);

    clear_win();
    init_subwins();
    cup_output();
    score_output();
    my_figure_output(my_fig);
    next_figure_output(next_fig);
    help_output();

    timeout(5);

    int good_op;
    char *ptr = NULL;
    int num_of_points;
    int **cup_ptr;
    for (;;) {
        int ch = getch();
        switch (ch) {
        case 122:
        case KEY_UP:
        case KEY_DOWN:
        case KEY_LEFT:
        case KEY_RIGHT:
        case ' ':
        case 27:
            sendto_icmp(get_server_fd(), serv, st_key, (char *)&ch, sizeof(ch));
            break;
        default:
            break;
        }
        ret = recvfrom_time(sock_out, serv, buf, get_cur_time_ms(), 15);
        if (ret != -1) {
            if (icmp_check(((Ip_pkt_t *)buf))) {
                if (icmp_check((Ip_pkt_t *)buf)) {
                    if (DEBUG) {
                        fprintf(stderr, "handler.c: c_h(): id - %d, ip - %s\n",
                                ((Ip_pkt_t *)buf)
                                    ->ping_pkt.hdr.icmp_hun.ih_idseq.icd_id,
                                inet_ntoa(((Ip_pkt_t *)buf)->ip_hdr.ip_src));
                    }
                }
                switch (
                    ((Ip_pkt_t *)buf)->ping_pkt.hdr.icmp_hun.ih_idseq.icd_id) {
                case st_serv_move:
                    good_op = 1;
                    figure_print_del(fig);
                    ptr = &(((Ip_pkt_t *)buf)->ping_pkt.data[TET_LEN + 1]);
                    fig.index_figure = ((Figure_send *)ptr)->fig.index_figure;
                    fig.turn_point_index =
                        ((Figure_send *)ptr)->fig.turn_point_index;
                    set_score(((Figure_send *)ptr)->score);
                    num_of_points = ((Figure_send *)ptr)->fig_size;
                    ptr = &(
                        ((Ip_pkt_t *)buf)
                            ->ping_pkt.data[TET_LEN + 1 + sizeof(Figure_send)]);
                    memcpy(fig.point, ptr,
                           num_of_points * sizeof(Figure_point));
                    break;
                case st_serv_down:
                    good_op = 1;
                    if (DEBUG) {
                        fprintf(stderr, "Size of cup msg: %d\n", ret);
                    }

                    figure_print_del(fig);
                    ptr = &(((Ip_pkt_t *)buf)->ping_pkt.data[TET_LEN + 1]);
                    cur_fig_id = next_fig_id;
                    next_fig_id = ((Cup_send *)ptr)->next_fig_id;
                    set_score(((Cup_send *)ptr)->score);
                    ptr =
                        &(((Ip_pkt_t *)buf)
                              ->ping_pkt.data[TET_LEN + 1 + sizeof(Cup_send)]);
                    cup_ptr = get_cup();
                    for (int i = 0; i < HEIGHT_CUP; ++i) {
                        for (int j = 0; j < WIDTH_CUP; ++j) {
                            cup_ptr[i][j] = ptr[i * WIDTH_CUP + j];
                        }
                    }

                    copy_by_figure_id(&fig, cur_fig_id);
                    copy_by_figure_id(&next_fig, next_fig_id);
                    if (!get_key_please_ch_win_size()) {
                        next_figure_output(next_fig);
                        cup_output();
                    }
                    break;
                case st_serv_quit:
                    good_op = -3;
                    break;
                default:
                    good_op = 0;
                    break;
                }
                if (good_op > 0) {
                    ret = sendto_icmp(sock_out, serv, st_check_answer, "ok", 0);
                    if (ret <= 0) {
                        fprintf(
                            stderr,
                            "handler.c: client_handler(): sendto_icmp(): -1");
                        perror("ERR: sendto_icmp()\n");
                        // good_op = -3;
                        break;
                    }
                    if (!get_key_please_ch_win_size()) {
                        score_output();
                    }
                    figure_print_add(fig);
                }
                if (good_op == -3) {
                    break;
                }
            }
        }
    }

    timeout(-1);
    fprintf(stderr, "client_serv: (1)\n");
    remove_figure(&fig);
    fprintf(stderr, "client_serv: (2)\n");
    remove_figure(&my_fig);
    fprintf(stderr, "client_serv: (3)\n");
    remove_figure(&next_fig);
    fprintf(stderr, "client_serv: (4)\n");
    fprintf(stderr, "client_serv: (5)\n");
    close(sock_out);
    free_subwins();
    fprintf(stderr, "client_serv: (6)\n");

    free(buf);
    set_change_handler(MENU);

    return 0;
}

int lobby_handler()
{
    int ret = 0;

    clear_win();
    allocate_cup();

    if (role != r_client) {
        set_change_handler(LOBBY);
        init_server();
        init_listen();
        while (1) {
            if (ip_written) {
                break;
            }
            usleep(20);
        }

        timeout(10);
        lobby_output();
        int key_exit = 0;
        while (!key_exit) {
            if (update_lobby) {
                lobby_output();
                update_lobby = 0;
            }
            int ch = getch();
            switch (ch) {
            case 10:
                key_listen = 1;
                while (serv_cli_fig_id < 0) {
                    usleep(20);
                }
                start_game = 1;
                client_handler();
                start_game = 0;
                ip_written = 0;
                pthread_join(tid_listen, NULL);
                key_exit = 1;
                break;
            case 27:
                start_game = 0;
                key_exit = 1;
                break;
            }
            if (get_num_of_players() != get_count_players()) {
                set_count_players(get_num_of_players());
                update_lobby = 1;
            }
        }
    } else {
        set_change_handler(CLIENT);
        wait_output();
        ret = client_handler();
    }

    timeout(-1);

    remove_cup();
    set_change_handler(MENU);

    if (ret == _err_failed) {
        return _err_failed;
    }

    return 0;
}

int menu_handler()
{
    srand(rand_num);
    int pos_cur = 1, key_exit = 0;
    int ret;
    struct sockaddr_in server;
    memset(&server, 0, sizeof(struct sockaddr_in));
    int err_out = open("resource/err_out.txt", O_WRONLY | O_CREAT, 00666);
    dup2(err_out, 2);

    init_interface();
    init_win();
    set_change_handler(MENU);

    while (!key_exit) {
        menu_output(COUNT_OPTS, pos_cur);
        int ch = getch();
        switch (ch) {
        case KEY_UP:
            pos_cur = pos_cur > 1 ? pos_cur - 1 : COUNT_OPTS;
            break;
        case KEY_DOWN:
            pos_cur = pos_cur < COUNT_OPTS ? pos_cur + 1 : 1;
            break;
        case 10:
            init_figure();
            switch (pos_cur) {
            case 1:
                role = r_serv;
                lobby_handler();
                if (get_score()) {
                    set_change_handler(FINISH);
                    finish_output();
                    set_score(0);
                    sleep(2);
                    getch();
                }
                set_change_handler(MENU);
                memset(get_server_ip(), 0, 30);
                break;
            case 2:
                role = r_client;
                ret = lobby_handler();
                if (ret != _err_failed && get_score()) {
                    set_change_handler(FINISH);
                    finish_output();
                    sleep(2);
                    getch();
                }
                set_change_handler(MENU);
                memset(get_server_ip(), 0, 30);
                break;
            case 3:
                key_exit = 1;
                break;
            default:
                break;
            }
            delete_rbtree(get_rbt());
        default:
            break;
        }
    }

    close(err_out);
    free_win();
    endwin();
    return 0;
}
