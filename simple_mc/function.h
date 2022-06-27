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
void printLeftWindow(WINDOW *left_win, struct dirent **data, int highlight, int n);
void printRightWindow(WINDOW *right_win, struct dirent **data, int highlight, int n);