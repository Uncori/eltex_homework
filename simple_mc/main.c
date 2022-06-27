#include <dirent.h>
#include <fcntl.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define PWD_BUF_SIZE 1024
#define FILE_BUF_SIZE 4096

char *time_my(struct stat *st);
void sig_winch(int signo) {
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
    resizeterm(size.ws_row, size.ws_col);
}
void print_menu(WINDOW *left_win, WINDOW *ringht_win, struct dirent **data, int highlight, int n);

int main() {
    WINDOW *left_win, *ringht_win;
    int c;

    initscr();
    signal(SIGWINCH, sig_winch);

    cbreak();
    noecho();
    curs_set(0);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);

    ringht_win = newwin(LINES, COLS / 2, 0, COLS / 2);
    left_win = newwin(LINES, COLS / 2, 0, 0);

    char *cwd = getcwd(NULL, 0);

    struct dirent **namelist;
    int n = scandir(cwd, &namelist, NULL, alphasort);
    int highlight = n;
    free(cwd);

    keypad(left_win, TRUE);
    refresh();

    print_menu(left_win, ringht_win, namelist, highlight, n);
    while (1) {
        c = wgetch(left_win);
        switch (c) {
            case KEY_DOWN:
                if (highlight == 1)
                    highlight = n;
                else
                    --highlight;
                break;
            case KEY_UP:
                if (highlight == n)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10:
                wclear(left_win);
                char *buf = getcwd(NULL, 0);
                strcat(buf, "/");
                strcat(buf, namelist[highlight - 1]->d_name);
                chdir(buf);
                while (n--) {
                    free(namelist[n]);
                }
                free(namelist);

                n = scandir(buf, &namelist, NULL, alphasort);
                highlight = n;

                free(buf);
                break;
            case 'a':
                exit(EXIT_SUCCESS);
            default:
                refresh();
                break;
        }
        print_menu(left_win, ringht_win, namelist, highlight, n);
    }
    while (n--) {
        free(namelist[n]);
    }
    free(namelist);
    delwin(left_win);
    delwin(ringht_win);
    endwin();
    return 0;
}

void print_menu(WINDOW *left_win, WINDOW *ringht_win, struct dirent **data, int highlight, int n) {
    int x = 1, y = 1;

    box(left_win, 0, 0);
    box(ringht_win, 0, 0);
    struct stat st;

    wbkgd(left_win, COLOR_PAIR(1));
    wbkgd(ringht_win, COLOR_PAIR(1));

    for (int i = n - 1; i >= 0; --i) {
        stat(data[i]->d_name, &st);
        char *time = time_my(&st);
        if (highlight == i + 1) {
            wattron(left_win, A_REVERSE);
            mvwprintw(left_win, y, x, data[i]->d_name);
            mvwprintw(left_win, y, (COLS / 2) - 24, "%d", st.st_size);
            mvwprintw(left_win, y, (COLS / 2) - 14, "%s", time);
            wattroff(left_win, A_REVERSE);
        } else {
            wrefresh(left_win);
            mvwprintw(left_win, y, x, data[i]->d_name);
            mvwprintw(left_win, y, (COLS / 2) - 24, "%d", st.st_size);
            mvwprintw(left_win, y, (COLS / 2) - 14, "%s", time);
        }
        free(time);
        ++y;
    }
}
char *time_my(struct stat *st) {
    struct tm *u;
    u = localtime(&st->st_ctime);
    char f[40];
    strftime(f, 40, "%b %d %H:%M", u);
    char *tmp = (char *)malloc(sizeof(f));
    strcpy(tmp, f);
    return tmp;
}
