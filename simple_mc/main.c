#include "function.h"

int main() {
    WINDOW *left_win, *right_win;
    int c = 0, flagWindow = 0;

    initscr();

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

                    char *leftBuff = getcwd(NULL, 0);
                    strcat(leftBuff, "/");
                    strcat(leftBuff, leftNamelist[highlightLeft - 1]->d_name);

                    if (chdir(leftBuff) == 0) {
                        while (countLeftDir--) {
                            free(leftNamelist[countLeftDir]);
                        }
                        free(leftNamelist);

                        countLeftDir = scandir(leftBuff, &leftNamelist, NULL, alphasort);
                        highlightLeft = countLeftDir;

                        free(leftBuff);
                    } else {
                        free(leftBuff);
                    }
                    break;
                } else {
                    wclear(right_win);
                    char *rightBuff = getcwd(NULL, 0);
                    strcat(rightBuff, "/");
                    strcat(rightBuff, rightNamelist[highlightRight - 1]->d_name);

                    if (chdir(rightBuff) == 0) {
                        while (countRightDir--) {
                            free(rightNamelist[countRightDir]);
                        }
                        free(rightNamelist);

                        countRightDir = scandir(rightBuff, &rightNamelist, NULL, alphasort);
                        highlightRight = countRightDir;

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

    free(curentDir);

    delwin(left_win);
    delwin(right_win);
    endwin();
    return 0;
}