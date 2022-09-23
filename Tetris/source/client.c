#include "../header/client.h"
#include "../header/interface.h"
#include "../header/server.h"
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

static int sock_serv;
static struct sockaddr_in serv_addr;

int init_client(int role, char *server_ip, int *my_fig_id, int *cur_fig_id,
                int *next_fig_id)
{
    char buf[SERV_BUF_SIZE];
    int get_len;

    sock_serv = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock_serv < 0) {
        perror("socket(): ");

        return _err_failed;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = 0;
    fprintf(stderr, "Server ip - %s\n", server_ip);
    if (!inet_aton(server_ip, (struct in_addr *)&serv_addr.sin_addr.s_addr)) {
        perror("inet_aton(): ");
        close(sock_serv);

        return _err_failed;
    }
    unsigned int size = sizeof(serv_addr);

    if (role != r_serv) {
        if (sendto_icmp(sock_serv, serv_addr, st_lobby_connect,
                        "May I come in?", 0) <= 0) {
            perror("sendto(): ");
            close(sock_serv);

            return _err_failed;
        }

        ulong start_time = get_cur_time_ms();

        while (1) {

            get_len =
                recvfrom_time(sock_serv, serv_addr, buf, start_time, 5000);
            if (get_len <= 0) {
                perror("client.c: recvfrom(): get_my_fig\n");
                close(sock_serv);
                return _err_failed;
            }

            if (icmp_check((Ip_pkt_t *)buf) &&
                (((Ip_pkt_t *)buf)->ping_pkt.hdr.icmp_hun.ih_idseq.icd_id ==
                 st_answer)) {

                *my_fig_id =
                    *((int *)&(((Ip_pkt_t *)buf)->ping_pkt.data[TET_LEN + 1]));
                fprintf(stderr, "my figure id = %d\n", *my_fig_id);

                break;
            } else {
                fprintf(stderr, "client.c: wrong package\n");
                continue;
            }
        }
    }

    while (1) {

        get_len = recvfrom(sock_serv, buf, SERV_BUF_SIZE, RECV_FLAGS,
                           (struct sockaddr *)&serv_addr, &size);
        if (get_len <= 0) {
            perror("recvfrom(): ");
            close(sock_serv);
            return _err_failed;
        }
        if (icmp_check((Ip_pkt_t *)buf)) {
            fprintf(stderr, "clientc: init_client(): id - %d, ip - %s\n",
                    ((Ip_pkt_t *)buf)->ping_pkt.hdr.icmp_hun.ih_idseq.icd_id,
                    inet_ntoa(((Ip_pkt_t *)buf)->ip_hdr.ip_src));
        }
        if (icmp_check((Ip_pkt_t *)buf) &&
            (((Ip_pkt_t *)buf)->ping_pkt.hdr.icmp_hun.ih_idseq.icd_id ==
             st_check)) {
            fprintf(stderr, "Server request check.\n");
            get_len =
                sendto_icmp(sock_serv, serv_addr, st_check_answer, "AAAA", 0);
            if (get_len <= 0) {
                perror("sendto_icmp(): ");
                close(sock_serv);
                return _err_failed;
            }
            break;
        } else {
            continue;
        }
    }

    *cur_fig_id =
        ((Figure_info *)(&(((Ip_pkt_t *)buf)->ping_pkt.data[TET_LEN + 1])))
            ->cur;
    *next_fig_id =
        ((Figure_info *)(&(((Ip_pkt_t *)buf)->ping_pkt.data[TET_LEN + 1])))
            ->next;
    close(sock_serv);

    return 0;
}
