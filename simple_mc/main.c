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

char *printTime(struct stat *st);
void sig_winch(int signo);
void printLeftWindow(WINDOW *left_win, struct dirent **data, int highlight, int n);
void printRightWindow(WINDOW *right_win, struct dirent **data, int highlight, int n);

int main() {
    WINDOW *left_win, *right_win;
    int c = 0, flagWindow = 0;

    initscr();
    signal(SIGWINCH, sig_winch);

    cbreak();
    noecho();
    curs_set(0);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);

    right_win = newwin(LINES, COLS / 2, 0, COLS / 2);
    left_win = newwin(LINES, COLS / 2, 0, 0);

    char *curentDir = getcwd(NULL, 0);

    struct dirent **leftNamelist;
    struct dirent **rightNamelist;

    int countLeftDir = scandir(curentDir, &leftNamelist, NULL, alphasort);
    int countRightDir = scandir(curentDir, &rightNamelist, NULL, alphasort);

    int highlightLeft = 0, highlightRight = 0;
    highlightLeft = countLeftDir;
    highlightRight = countRightDir;

    free(curentDir);

    keypad(left_win, TRUE);
    keypad(right_win, TRUE);

    refresh();

    printLeftWindow(left_win, leftNamelist, highlightLeft, countLeftDir);

    printRightWindow(right_win, rightNamelist, highlightRight, countRightDir);

    while (c != 'q') {
        if (flagWindow == 0) {
            c = wgetch(left_win);
        } else {
            c = wgetch(right_win);
        }
        switch (c) {
            case KEY_DOWN:
                if (flagWindow == 0) {
                    if (highlightLeft == 1) {
                        highlightLeft = countLeftDir;
                    } else {
                        --highlightLeft;
                    }
                } else {
                    if (highlightRight == 1) {
                        highlightRight = countRightDir;
                    } else {
                        --highlightRight;
                    }
                }
                break;
            case KEY_UP:
                if (flagWindow == 0) {
                    if (highlightLeft == countLeftDir) {
                        highlightLeft = 1;
                    } else {
                        ++highlightLeft;
                    }
                } else {
                    if (highlightRight == countRightDir) {
                        highlightRight = 1;
                    } else {
                        ++highlightRight;
                    }
                }
                break;
            case KEY_RIGHT:
                flagWindow = 1;
                break;
            case KEY_LEFT:
                flagWindow = 0;
                break;
            case 10:
                if (flagWindow == 0) {
                    wclear(left_win);
                    char *buf = getcwd(NULL, 0);
                    strcat(buf, "/");
                    strcat(buf, leftNamelist[highlightLeft - 1]->d_name);
                    chdir(buf);
                    mvwprintw(left_win, 10, 10, buf);

                    while (countLeftDir--) {
                        free(leftNamelist[countLeftDir]);
                    }
                    free(leftNamelist);

                    countLeftDir = scandir(buf, &leftNamelist, NULL, alphasort);
                    highlightLeft = countLeftDir;

                    free(buf);
                    break;
                } else {
                    wclear(right_win);
                    char *buf = getcwd(NULL, 0);
                    strcat(buf, "/");
                    strcat(buf, rightNamelist[highlightRight - 1]->d_name);
                    chdir(buf);
                    mvwprintw(right_win, 10, 10, buf);
                    while (countRightDir--) {
                        free(rightNamelist[countRightDir]);
                    }
                    free(rightNamelist);

                    countRightDir = scandir(buf, &rightNamelist, NULL, alphasort);
                    highlightRight = countRightDir;

                    free(buf);
                    break;
                }
            default:
                refresh();
                break;
        }
        if (flagWindow == 0) {
            printLeftWindow(left_win, leftNamelist, highlightLeft, countLeftDir);
        } else {
            printRightWindow(right_win, rightNamelist, highlightRight, countRightDir);
        }
    }

    while (countLeftDir--) {
        free(leftNamelist[countLeftDir]);
    }
    free(leftNamelist);

    while (countRightDir--) {
        free(rightNamelist[countRightDir]);
    }
    free(rightNamelist);

    delwin(left_win);
    delwin(right_win);
    endwin();
    return 0;
}

void printLeftWindow(WINDOW *left_win, struct dirent **data, int highlight, int n) {
    int x = 1, y = 1;

    box(left_win, 0, 0);
    struct stat st;

    wbkgd(left_win, COLOR_PAIR(1));

    for (int i = n - 1; i >= 0; --i) {
        stat(data[i]->d_name, &st);
        char *time = printTime(&st);
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
void printRightWindow(WINDOW *right_win, struct dirent **data, int highlight, int n) {
    int x = 1, y = 1;

    box(right_win, 0, 0);
    struct stat st;

    wbkgd(right_win, COLOR_PAIR(1));

    for (int i = n - 1; i >= 0; --i) {
        stat(data[i]->d_name, &st);
        char *time = printTime(&st);
        if (highlight == i + 1) {
            wattron(right_win, A_REVERSE);
            mvwprintw(right_win, y, x, data[i]->d_name);
            mvwprintw(right_win, y, (COLS / 2) - 24, "%d", st.st_size);
            mvwprintw(right_win, y, (COLS / 2) - 14, "%s", time);
            wattroff(right_win, A_REVERSE);
        } else {
            wrefresh(right_win);
            mvwprintw(right_win, y, x, data[i]->d_name);
            mvwprintw(right_win, y, (COLS / 2) - 24, "%d", st.st_size);
            mvwprintw(right_win, y, (COLS / 2) - 14, "%s", time);
        }
        free(time);
        ++y;
    }
}
void sig_winch(int signo) {
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
    resizeterm(size.ws_row, size.ws_col);
}
char *printTime(struct stat *st) {
    struct tm *u;
    u = localtime(&st->st_ctime);
    char f[40];
    strftime(f, 40, "%b %d %H:%M", u);
    char *tmp = (char *)malloc(sizeof(f));
    strcpy(tmp, f);
    return tmp;
}
