#ifndef _PIPE_H
#define _PIPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void checkArgv(int *countPipe, int *indexPipe, int argc, char *argv[]);

void allocateAraay(const int *indexPipe, int argc, char *firstArr[],
                   char *secondArr[], char *argv[]);

void checkRes(const int *res);

void workWithPipe(int indexPipe, int argc, char *argv[]);

void workWithOutPipe(int argc, char *argv[]);

#endif  // _PIPE_H
