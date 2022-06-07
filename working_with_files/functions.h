#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FILE_NAME "test.txt"

void write_print(int fd, char *str);
void revers_output(int fd);

#endif  // _FUNCTION_H_
