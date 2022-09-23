#ifndef HANDLER_H
#define HANDLER_H

#define COUNT_OPTS 3

typedef struct {
    char *nickname;
    int figure_id;
} Users_data;

int init_listen();
int client_handler();
int lobby_handler();
int menu_handler();

#endif // HANDLER_H
