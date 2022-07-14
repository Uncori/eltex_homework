#include "function.h"

void printLeftWindow(WINDOW *left_win, struct dirent **data, int highlight, int n) {
    int x = 2, y = 2;
    box(left_win, 0, 0);
    struct stat st;

    mvwprintw(left_win, 1, 1, "Dir");
    mvwprintw(left_win, 1, (COLS / 2) - 24, "Size");
    mvwprintw(left_win, 1, (COLS / 2) - 14, "Modify time");

    wbkgd(left_win, COLOR_PAIR(1));

    for (int i = n - 1; i >= 0; --i) {
        stat(data[i]->d_name, &st);
        char *time = printTime(&st);

        if (highlight == i + 1) {
            if (S_ISDIR(st.st_mode)) {
                mvwprintw(left_win, y, 1, "/");
            }
            wattron(left_win, A_REVERSE);
            mvwprintw(left_win, y, x, data[i]->d_name);
            mvwprintw(left_win, y, (COLS / 2) - 24, "%d", st.st_size);
            mvwprintw(left_win, y, (COLS / 2) - 14, "%s", time);
            wattroff(left_win, A_REVERSE);
        } else {
            if (S_ISDIR(st.st_mode)) {
                mvwprintw(left_win, y, 1, "/");
            }
            mvwprintw(left_win, y, x, data[i]->d_name);
            mvwprintw(left_win, y, (COLS / 2) - 24, "%d", st.st_size);
            mvwprintw(left_win, y, (COLS / 2) - 14, "%s", time);
        }

        free(time);
        ++y;
    }
    wscrl(left_win, y);
    wrefresh(left_win);
}

void printRightWindow(WINDOW *right_win, struct dirent **data, int highlight, int n) {
    int x = 2, y = 2;
    mvwprintw(right_win, 1, 1, "Dir");
    mvwprintw(right_win, 1, (COLS / 2) - 24, "Size");
    mvwprintw(right_win, 1, (COLS / 2) - 14, "Modify time");

    box(right_win, 0, 0);
    struct stat st;

    wbkgd(right_win, COLOR_PAIR(1));

    for (int i = n - 1; i >= 0; --i) {
        stat(data[i]->d_name, &st);
        char *time = printTime(&st);
        if (highlight == i + 1) {
            if (S_ISDIR(st.st_mode)) {
                mvwprintw(right_win, y, 1, "/");
            }
            wattron(right_win, A_REVERSE);
            mvwprintw(right_win, y, x, data[i]->d_name);
            mvwprintw(right_win, y, (COLS / 2) - 24, "%d", st.st_size);
            mvwprintw(right_win, y, (COLS / 2) - 14, "%s", time);
            wattroff(right_win, A_REVERSE);
        } else {
            if (S_ISDIR(st.st_mode)) {
                mvwprintw(right_win, y, 1, "/");
            }
            mvwprintw(right_win, y, x, data[i]->d_name);
            mvwprintw(right_win, y, (COLS / 2) - 24, "%d", st.st_size);
            mvwprintw(right_win, y, (COLS / 2) - 14, "%s", time);
        }
        free(time);
        ++y;
    }
    wrefresh(right_win);
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

void mcRun() {
    WINDOW *left_win, *right_win;
    int c = 0, flagWindow = 0;

    initscr();

    cbreak();
    noecho();
    curs_set(0);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);

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
                wclear(left_win);

                char *leftBuff = getcwd(NULL, 0);
                // strncat(leftBuff, "/", 2);
                // strncat(leftBuff, leftNamelist[highlightLeft - 1]->d_name,
                //         strlen(leftNamelist[highlightLeft - 1]->d_name));

                // chdir(leftBuff);
                // // while (countLeftDir--) {
                // //     free(leftNamelist[countLeftDir]);
                // // }
                // // free(leftNamelist);

                // countLeftDir = scandir(leftBuff, &leftNamelist, NULL, alphasort);
                // highlightLeft = countLeftDir;

                // free(leftBuff);

                break;
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
    memClean(countLeftDir, countRightDir, left_win, leftNamelist, right_win, rightNamelist,
             curentDir);
}

void memClean(int countLeftDir, int countRightDir, WINDOW *left_win, struct dirent **leftNamelist,
              WINDOW *right_win, struct dirent **rightNamelist, char *curentDir) {
    while (countLeftDir--) {
        free(leftNamelist[countLeftDir]);
    }
    free(leftNamelist);

    while (countRightDir--) {
        free(rightNamelist[countRightDir]);
    }
    free(rightNamelist);
    free(curentDir);
    delwin(left_win);
    delwin(right_win);
    endwin();
}
