#ifndef _FUNCTION_H
#define _FUNCTION_H

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

#define maxPathLength 1024

typedef struct dataBase {
    WINDOW *left_win;
    WINDOW *right_win;
    struct dirent **leftNamelist;
    struct dirent **rightNamelist;
    int countLeftDir;
    int countRightDir;
    int highlightLeft;
    int highlightRight;
    char *curentDir;
} Data;

void mcRun();
char *printTime(struct stat *st);
void printLeftWindow(Data *base);
void printRightWindow(Data *base);
void memClean(Data *base);

#endif  // _FUNCTION_H
