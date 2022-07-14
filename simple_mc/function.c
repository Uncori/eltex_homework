#include "function.h"

void printLeftWindow(Data *base) {
    int x = 2, y = 2;
    box(base->left_win, 0, 0);
    struct stat st;

    mvwprintw(base->left_win, 1, 1, "Dir");
    mvwprintw(base->left_win, 1, (COLS / 2) - 24, "Size");
    mvwprintw(base->left_win, 1, (COLS / 2) - 14, "Modify time");

    wbkgd(base->left_win, COLOR_PAIR(1));

    for (int i = base->countLeftDir - 1; i >= 0; --i) {
        stat(base->leftNamelist[i]->d_name, &st);
        char *time = printTime(&st);

        if (base->highlightLeft == i + 1) {
            if (S_ISDIR(st.st_mode)) {
                mvwprintw(base->left_win, y, 1, "/");
            }
            wattron(base->left_win, A_REVERSE);
            mvwprintw(base->left_win, y, x, base->leftNamelist[i]->d_name);
            mvwprintw(base->left_win, y, (COLS / 2) - 24, "%d", st.st_size);
            mvwprintw(base->left_win, y, (COLS / 2) - 14, "%s", time);
            wattroff(base->left_win, A_REVERSE);
        } else {
            if (S_ISDIR(st.st_mode)) {
                mvwprintw(base->left_win, y, 1, "/");
            }
            mvwprintw(base->left_win, y, x, base->leftNamelist[i]->d_name);
            mvwprintw(base->left_win, y, (COLS / 2) - 24, "%d", st.st_size);
            mvwprintw(base->left_win, y, (COLS / 2) - 14, "%s", time);
        }

        free(time);
        ++y;
    }
    wscrl(base->left_win, y);
    wrefresh(base->left_win);
}

void printRightWindow(Data *base) {
    int x = 2, y = 2;
    mvwprintw(base->right_win, 1, 1, "Dir");
    mvwprintw(base->right_win, 1, (COLS / 2) - 24, "Size");
    mvwprintw(base->right_win, 1, (COLS / 2) - 14, "Modify time");

    box(base->right_win, 0, 0);
    struct stat st;

    wbkgd(base->right_win, COLOR_PAIR(1));

    for (int i = base->countRightDir - 1; i >= 0; --i) {
        stat(base->rightNamelist[i]->d_name, &st);
        char *time = printTime(&st);
        if (base->highlightRight == i + 1) {
            if (S_ISDIR(st.st_mode)) {
                mvwprintw(base->right_win, y, 1, "/");
            }
            wattron(base->right_win, A_REVERSE);
            mvwprintw(base->right_win, y, x, base->rightNamelist[i]->d_name);
            mvwprintw(base->right_win, y, (COLS / 2) - 24, "%d", st.st_size);
            mvwprintw(base->right_win, y, (COLS / 2) - 14, "%s", time);
            wattroff(base->right_win, A_REVERSE);
        } else {
            if (S_ISDIR(st.st_mode)) {
                mvwprintw(base->right_win, y, 1, "/");
            }
            mvwprintw(base->right_win, y, x, base->rightNamelist[i]->d_name);
            mvwprintw(base->right_win, y, (COLS / 2) - 24, "%d", st.st_size);
            mvwprintw(base->right_win, y, (COLS / 2) - 14, "%s", time);
        }
        free(time);
        ++y;
    }
    wrefresh(base->right_win);
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
    Data base;

    int c = 0, flagWindow = 0;

    initscr();

    cbreak();
    noecho();
    curs_set(0);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);

    base.right_win = newwin(LINES, COLS / 2, 0, COLS / 2);
    base.left_win = newwin(LINES, COLS / 2, 0, 0);

    base.curentDir = getcwd(NULL, 0);

    base.countLeftDir = scandir(base.curentDir, &base.leftNamelist, NULL, alphasort);
    base.countRightDir = scandir(base.curentDir, &base.rightNamelist, NULL, alphasort);

    base.highlightLeft = 0;
    base.highlightRight = 0;

    base.highlightLeft = base.countLeftDir;
    base.highlightRight = base.countRightDir;

    keypad(base.left_win, TRUE);
    keypad(base.right_win, TRUE);

    refresh();

    printLeftWindow(&base);
    printRightWindow(&base);

    while (c != 'q') {
        if (flagWindow == 0) {
            c = wgetch(base.left_win);
        } else {
            c = wgetch(base.right_win);
        }
        switch (c) {
            case KEY_DOWN:
                if (flagWindow == 0) {
                    if (base.highlightLeft == 1) {
                        base.highlightLeft = base.countLeftDir;
                    } else {
                        --base.highlightLeft;
                    }
                } else {
                    if (base.highlightRight == 1) {
                        base.highlightRight = base.countRightDir;
                    } else {
                        --base.highlightRight;
                    }
                }
                break;
            case KEY_UP:
                if (flagWindow == 0) {
                    if (base.highlightLeft == base.countLeftDir) {
                        base.highlightLeft = 1;
                    } else {
                        ++base.highlightLeft;
                    }
                } else {
                    if (base.highlightRight == base.countRightDir) {
                        base.highlightRight = 1;
                    } else {
                        ++base.highlightRight;
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
                    wclear(base.left_win);
                    char *leftBuff = getcwd(NULL, 0);
                    strncat(leftBuff, "/", 2);
                    strncat(leftBuff, base.leftNamelist[base.highlightLeft - 1]->d_name,
                            strlen(base.leftNamelist[base.highlightLeft - 1]->d_name));

                    if (chdir(leftBuff) == 0) {
                        while (base.countLeftDir--) {
                            free(base.leftNamelist[base.countLeftDir]);
                        }
                        free(base.leftNamelist);

                        base.countLeftDir = scandir(leftBuff, &base.leftNamelist, NULL, alphasort);
                        base.highlightLeft = base.countLeftDir;

                        free(leftBuff);
                    } else {
                        free(leftBuff);
                    }
                    break;
                } else {
                    wclear(base.right_win);
                    char *rightBuff = getcwd(NULL, 0);
                    strncat(rightBuff, "/", 2);
                    strncat(rightBuff, base.rightNamelist[base.highlightRight - 1]->d_name,
                            strlen(base.rightNamelist[base.highlightRight - 1]->d_name));

                    if (chdir(rightBuff) == 0) {
                        while (base.countRightDir--) {
                            free(base.rightNamelist[base.countRightDir]);
                        }
                        free(base.rightNamelist);

                        base.countRightDir =
                            scandir(rightBuff, &base.rightNamelist, NULL, alphasort);
                        base.highlightRight = base.countRightDir;

                        free(rightBuff);
                    } else {
                        free(rightBuff);
                    }
                    break;
                }
            default:
                refresh();
                break;
        }
        if (flagWindow == 0) {
            printLeftWindow(&base);
        } else {
            printRightWindow(&base);
        }
    }
    memClean(&base);
}

void memClean(Data *base) {
    while (base->countLeftDir--) {
        free(base->leftNamelist[base->countLeftDir]);
    }
    free(base->leftNamelist);

    while (base->countRightDir--) {
        free(base->rightNamelist[base->countRightDir]);
    }
    free(base->rightNamelist);
    free(base->curentDir);
    delwin(base->left_win);
    delwin(base->right_win);
    endwin();
}
