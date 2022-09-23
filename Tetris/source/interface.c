#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

#include "../header/cup.h"
#include "../header/figure.h"
#include "../header/inet.h"
#include "../header/interface.h"
#include "../header/rbtree.h"

static Window *win;
static struct winsize size;
static char *menu_opts = NULL, *help_opts = NULL;
static int change_handler = 0, count_opts_temp = 0, pos_cur_temp = 0,
           multi_height_cup = 0, multi_width_cup = 0, update_wins = 0,
           key_please_ch_win_size = 0;

int set_change_handler(Wins win)
{
    change_handler = win;

    return 0;
}

int please_ch_win_size_output()
{
    wclear(win->main);
    wmove(win->main, size.ws_row / 2, size.ws_col / 2 - 13);
    wprintw(win->main, "Please change window size");
    wrefresh(win->main);

    return 0;
}

int wait_output()
{
    wclear(win->main);
    if (!strlen(get_server_ip())) {
        nocbreak();
        echo();
        wmove(win->main, size.ws_row / 2 - 1, size.ws_col / 2 - 8);
        wprintw(win->main, "Enter server IP: ");
        wrefresh(win->main);
        wscanw(win->main, "%s", get_server_ip());
        cbreak();
        noecho();
    }
    if (strlen(get_server_ip())) {
        wmove(win->main, size.ws_row / 2, size.ws_col / 2 - 8);
        wprintw(win->main, "Wait start game");
        wrefresh(win->main);
    }

    return 0;
}

int finish_output()
{
    wclear(win->main);
    wmove(win->main, size.ws_row / 2, size.ws_col / 2 - 8);
    wprintw(win->main, "SCORE %d", get_score());
    wmove(win->main, size.ws_row / 2 + 1, size.ws_col / 2 - 8);
    wprintw(win->main, "Press any key...");
    wrefresh(win->main);

    return 0;
}

int change_multi_wins()
{
    multi_height_cup = 0;
    multi_width_cup = 0;
    key_please_ch_win_size = 0;

    for (int i = 0; i <= MAX_MULTI_HEIGHT_CUP; ++i) {
        if (HEIGHT_CUP * i + 2 <= size.ws_row) {
            multi_height_cup = i;
        } else {
            break;
        }
    }

    for (int i = 0; i <= MAX_MULTI_WIDTH_CUP; ++i) {
        if (WIDTH_CUP * i + FIGURE_SIZE * i * 2 + 2 * i < size.ws_col) {
            multi_width_cup = i;
        } else {
            break;
        }
    }

    if (!multi_height_cup || !multi_width_cup) {
        key_please_ch_win_size = 1;
    }

    return 0;
}

int figure_print_add(Figure figure)
{
    for (int i = 0; i < figure.count_point; ++i) {
        if (figure.point[i].x < 0) {
            continue;
        }
        wattron(win->cup, COLOR_PAIR(figure.index_figure / 8 > 0
                                         ? figure.index_figure % 8 + 1
                                         : figure.index_figure));
        for (int k = 0; k < multi_height_cup; k++) {
            wmove(win->cup, figure.point[i].x * multi_height_cup + k,
                  figure.point[i].y * multi_width_cup);
            for (int j = 0; j < multi_width_cup; ++j) {
                wprintw(win->cup, " ");
            }
        }
        wattron(win->cup, COLOR_PAIR(9));
    }

    wrefresh(win->cup);
    return 0;
}

int figure_print_del(Figure figure)
{
    for (int i = 0; i < figure.count_point; ++i) {
        if (figure.point[i].x < 0) {
            continue;
        }
        for (int k = 0; k < multi_height_cup; k++) {
            wmove(win->cup, figure.point[i].x * multi_height_cup + k,
                  figure.point[i].y * multi_width_cup);
            for (int j = 0; j < multi_width_cup; ++j) {
                wprintw(win->cup, " ");
            }
        }
    }

    wrefresh(win->cup);
    return 0;
}

int lobby_output()
{
    wclear(win->main);
    wmove(win->main, size.ws_row / 2 - 1, size.ws_col / 2 - 8);
    wprintw(win->main, "Server IP: %s\n", get_server_ip());
    wmove(win->main, size.ws_row / 2, size.ws_col / 2 - 8);
    wprintw(win->main, "Count players: %d\n", get_count_players());
    wmove(win->main, size.ws_row / 2 + 1, size.ws_col / 2 - 8);
    wprintw(win->main, "Press any key...");

    wrefresh(win->main);
    return 0;
}

int menu_output(int count_opts, int pos_cur)
{
    char *istr = NULL,
         *temp_menu_opts = (char *)malloc(TEXT_SIZE * sizeof(char));

    count_opts_temp = count_opts;
    pos_cur_temp = pos_cur;

    wclear(win->main);
    memcpy(temp_menu_opts, menu_opts, TEXT_SIZE * sizeof(char));
    istr = strtok(temp_menu_opts, "\n");
    for (int i = 0; i <= count_opts; ++i) {
        wmove(win->main, size.ws_row / 2 - count_opts / 2 + i,
              size.ws_col / 2 - strlen(istr) / 2);
        if (pos_cur == i) {
            wattron(win->main, COLOR_PAIR(9));
        }
        wprintw(win->main, "%s", istr);
        if (pos_cur == i) {
            wattron(win->main, COLOR_PAIR(8));
        }
        istr = strtok(NULL, "\n");
    }

    free(temp_menu_opts);
    wrefresh(win->main);
    return 0;
}

int clear_win()
{
    wclear(win->main);
    wrefresh(win->main);

    return 0;
}

int cup_output()
{
    int **cup = get_cup();

    wclear(win->cup);
    for (int i = 0; i < HEIGHT_CUP; ++i) {
        for (int j = 0; j < WIDTH_CUP; ++j) {
            if (cup[i][j] > 0) {
                wattron(win->cup,
                        COLOR_PAIR(cup[i][j] / 8 > 0 ? cup[i][j] % 8 + 1
                                                     : cup[i][j]));
                for (int k = 0; k < multi_height_cup; ++k) {
                    wmove(win->cup, i * multi_height_cup + k,
                          j * multi_width_cup);
                    for (int z = 0; z < multi_width_cup; ++z) {
                        wprintw(win->cup, " ");
                    }
                }
            }
        }
    }
    wattron(win->cup, COLOR_PAIR(9));
    wrefresh(win->cup);

    return 0;
}

int score_output()
{
    wmove(win->score, 0, 0);
    wprintw(win->score, "SCORE: %d", get_score());
    wrefresh(win->score);

    return 0;
}

int my_figure_output(Figure figure)
{
    wclear(win->my_figure);
    shift_down(&figure);
    wmove(win->my_figure, 0, 3);
    if (multi_width_cup > 3) {
        wprintw(win->my_figure, "MY FIGURE");
    }
    for (int i = 0; i < figure.count_point; ++i) {
        if (figure.point[i].x < 0) {
            continue;
        }
        wattron(win->my_figure, COLOR_PAIR(figure.index_figure / 8 > 0
                                               ? figure.index_figure % 8 + 1
                                               : figure.index_figure));
        for (int k = 0; k < multi_height_cup; ++k) {
            wmove(win->my_figure, figure.point[i].x * multi_height_cup + k + 1,
                  figure.point[i].y * multi_width_cup);
            for (int j = 0; j < multi_width_cup; ++j) {
                wprintw(win->my_figure, " ");
            }
        }
        wattron(win->my_figure, COLOR_PAIR(9));
    }
    wrefresh(win->my_figure);

    return 0;
}

int next_figure_output(Figure figure)
{
    wclear(win->next_figure);
    shift_down(&figure);
    wmove(win->next_figure, 0, 2);
    if (multi_width_cup > 3) {
        wprintw(win->next_figure, "NEXT FIGURE");
    }
    for (int i = 0; i < figure.count_point; ++i) {
        if (figure.point[i].x < 0) {
            continue;
        }
        wattron(win->next_figure, COLOR_PAIR(figure.index_figure / 8 > 0
                                                 ? figure.index_figure % 8 + 1
                                                 : figure.index_figure));
        for (int k = 0; k < multi_height_cup; ++k) {
            wmove(win->next_figure,
                  figure.point[i].x * multi_height_cup + k + 1,
                  figure.point[i].y * multi_width_cup);
            for (int j = 0; j < multi_width_cup; ++j) {
                wprintw(win->next_figure, " ");
            }
        }
        wattron(win->next_figure, COLOR_PAIR(9));
    }
    wrefresh(win->next_figure);

    return 0;
}

int help_output()
{
    char *istr = NULL,
         *temp_help_opts = (char *)malloc(TEXT_SIZE * sizeof(char));

    wclear(win->help);
    memcpy(temp_help_opts, help_opts, TEXT_SIZE * sizeof(char));
    istr = strtok(temp_help_opts, "\n");
    wprintw(win->help, "%s\n", istr);
    for (int i = 0; i < COUNT_HELP; ++i) {
        if (multi_width_cup == 1) {
            istr = strtok(NULL, " ");
            wprintw(win->help, "%s\n", istr);
            istr = strtok(NULL, "\n");
        } else {
            istr = strtok(NULL, "\n");
            wprintw(win->help, "%s\n", istr);
        }
    }
    wrefresh(win->help);

    return 0;
}

void sig_winch()
{
    ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
    resizeterm(size.ws_row, size.ws_col);
    refresh();
    change_multi_wins();
    update_wins = 1;
    update_winch();
}

int get_key_please_ch_win_size() { return key_please_ch_win_size; }

int update_winch()
{
    init_win();
    switch (change_handler) {
    case MENU:
        menu_output(count_opts_temp, pos_cur_temp);
        break;
    case LOBBY:
        lobby_output();
        break;
    case GAME:
        if (key_please_ch_win_size) {
            please_ch_win_size_output();
        } else {
            clear_win();
            init_subwins();
            cup_output();
            score_output();
            Figure my_fig, next_fig;
            allocate_figure(&my_fig, 5);
            allocate_figure(&next_fig, 5);
            copy_by_figure_id(&my_fig, get_my_figure_id());
            copy_by_figure_id(&next_fig, get_next_figure_id());
            my_figure_output(my_fig);
            next_figure_output(next_fig);
            help_output();
        }
        break;
    case CLIENT:
        wait_output();
        break;
    case FINISH:
        finish_output();
        break;
    default:
        break;
    }

    update_wins = 0;

    return 0;
}

int init_interface()
{
    initscr();
    cbreak();
    noecho();
    curs_set(false);
    keypad(stdscr, true);
    start_color();
    signal(SIGWINCH, sig_winch);
    refresh();

    init_pair(1, COLOR_BLUE, COLOR_BLUE);
    init_pair(2, COLOR_RED, COLOR_RED);
    init_pair(3, COLOR_GREEN, COLOR_GREEN);
    init_pair(4, COLOR_RED, COLOR_RED);
    init_pair(5, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(6, COLOR_CYAN, COLOR_CYAN);
    init_pair(7, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(8, COLOR_WHITE, COLOR_BLACK);
    init_pair(9, COLOR_BLACK, COLOR_WHITE);

    win = (Window *)calloc(1, sizeof(Window));
    ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
    resizeterm(size.ws_row, size.ws_col);
    refresh();
    change_multi_wins();

    menu_opts = (char *)calloc(TEXT_SIZE, sizeof(char));
    FILE *menu_file = fopen("resource/menu_opts.txt", "r");
    for (int i = 0; !feof(menu_file) && i < TEXT_SIZE; ++i) {
        menu_opts[i] = getc(menu_file);
    }
    fclose(menu_file);

    help_opts = (char *)calloc(TEXT_SIZE, sizeof(char));
    FILE *help_file = fopen("resource/help_opts.txt", "r");
    for (int i = 0; !feof(help_file) && i < TEXT_SIZE; ++i) {
        help_opts[i] = getc(help_file);
    }
    fclose(help_file);

    return 0;
}

int init_win()
{
    win->main = newwin(size.ws_row, size.ws_col, 0, 0);
    wbkgd(win->main, COLOR_PAIR(8));

    return 0;
}

int free_win()
{
    delwin(win->main);
    free(win);
    free(menu_opts);

    return 0;
}

int init_subwins()
{
    win->cup = derwin(win->main, HEIGHT_CUP * multi_height_cup,
                      WIDTH_CUP * multi_width_cup,
                      (size.ws_row - HEIGHT_CUP * multi_height_cup) / 2 + 1,
                      size.ws_col / 2 - WIDTH_CUP * multi_width_cup);
    win->score = derwin(win->main, 1, FIGURE_SIZE * multi_width_cup * 2 + 2,
                        (size.ws_row - HEIGHT_CUP * multi_height_cup) / 2 + 1,
                        size.ws_col / 2 + 2);
    win->my_figure =
        derwin(win->main, FIGURE_SIZE * multi_height_cup + 1,
               FIGURE_SIZE * multi_width_cup,
               (size.ws_row - HEIGHT_CUP * multi_height_cup) / 2 + 3,
               size.ws_col / 2 + 2);
    win->next_figure =
        derwin(win->main, FIGURE_SIZE * multi_height_cup + 1,
               FIGURE_SIZE * multi_width_cup,
               (size.ws_row - HEIGHT_CUP * multi_height_cup) / 2 + 3,
               size.ws_col / 2 + FIGURE_SIZE * multi_width_cup + 4);
    win->help = derwin(win->main,
                       HEIGHT_CUP * multi_height_cup -
                           FIGURE_SIZE * multi_height_cup - 4,
                       FIGURE_SIZE * multi_width_cup * 2 + 3,
                       (size.ws_row - HEIGHT_CUP * multi_height_cup) / 2 +
                           FIGURE_SIZE * multi_height_cup + 4,
                       size.ws_col / 2 + 2);
    win->alert = derwin(win->main, 1, FIGURE_SIZE * 2 + 1,
                        (size.ws_row - HEIGHT_CUP * multi_height_cup) / 2 - 1,
                        size.ws_col / 2 - FIGURE_SIZE);

    wbkgd(win->cup, COLOR_PAIR(9));
    wbkgd(win->score, COLOR_PAIR(9));
    wbkgd(win->my_figure, COLOR_PAIR(9));
    wbkgd(win->next_figure, COLOR_PAIR(9));
    wbkgd(win->help, COLOR_PAIR(8));
    wbkgd(win->alert, COLOR_PAIR(9));

    return 0;
}

int free_subwins()
{
    delwin(win->cup);
    delwin(win->score);
    delwin(win->my_figure);
    delwin(win->next_figure);
    delwin(win->help);
    delwin(win->alert);

    return 0;
}
