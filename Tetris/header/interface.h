#ifndef INTERFACE_H
#define INTERFACE_H

#include <curses.h>

#include "figure.h"

#define TEXT_SIZE 256
#define FIGURE_SIZE 4
#define COUNT_HELP 7
#define MAX_MULTI_HEIGHT_CUP 2
#define MAX_MULTI_WIDTH_CUP 4

typedef struct {
    WINDOW *main;
    WINDOW *cup;
    WINDOW *score;
    WINDOW *my_figure;
    WINDOW *next_figure;
    WINDOW *help;
    WINDOW *alert;
} Window;

typedef enum {
    MENU = 0,
    LOBBY = 1,
    GAME = 2,
    PLEASE_CH_WIN_SIZE = 3,
    CLIENT = 4,
    FINISH = 5
} Wins;

int set_change_handler(Wins win);
int please_ch_win_size_output();
int wait_output();
int finish_output();
int change_multi_wins();
int figure_print_add(Figure figure);
int figure_print_del(Figure figure);
int lobby_output();
int menu_output(int count_opts, int pos_cur);
int clear_win();
int cup_output();
int score_output();
int my_figure_output(Figure figure);
int next_figure_output(Figure figure);
int help_output();
void sig_winch();
int get_key_please_ch_win_size();
int update_winch();
int init_interface();
int init_win();
int free_win();
int init_subwins();
int free_subwins();

#endif
