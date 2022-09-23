#ifndef INET_H
#define INET_H

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>

#define BUF_SIZE 256
#define PORT 7777
#define BROADCAST_IP "224.0.0.1"

typedef struct {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t seq;
} Icmp_header;

typedef struct {
    uint8_t version_ihl;
    uint8_t ds;
    uint16_t length;
    uint16_t id;
    uint16_t flag_offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t source_ip;
    uint32_t dest_ip;
} Ip_header;

typedef struct {
    struct icmp icmphdr;
    char data[BUF_SIZE];
} Icmp_packet;

typedef struct {
    Ip_header iphdr;
    Icmp_packet icmp_pckt;
} Ip_packet;

typedef struct {
    struct sockaddr_in addr;
    int sock_fd;
    int figure_id;
} Players;

int get_server_fd();
int set_server_fd(int value);
char *get_server_ip();
int get_count_players();
int set_count_players(int value);
int init_server();

#endif // INET_H
