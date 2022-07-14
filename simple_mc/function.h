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

void mcRun();
char *printTime(struct stat *st);
void printLeftWindow(WINDOW *left_win, struct dirent **data, int highlight, int n);
void printRightWindow(WINDOW *right_win, struct dirent **data, int highlight, int n);
void memClean(int countLeftDir, int countRightDir, WINDOW *left_win, struct dirent **leftNamelist,
              WINDOW *right_win, struct dirent **rightNamelist, char *curentDir);

#endif  // _FUNCTION_H
