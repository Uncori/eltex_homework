#ifndef CLIENT_H
#define CLIENT_H

#include "figure.h"

int init_client(int role, char *server_ip, int *my_fig_id, int *cur_fig_id,
                int *next_fig_id);

#endif // CLIENT_H
