#include "functions.h"

void write_print(int fd, char *str) {
    char *c = (char *)malloc(255 * sizeof(char));
    write(fd, str, strlen(str));
    lseek(fd, 0, SEEK_SET);
    read(fd, c, strlen(str));
    printf("%s\n", c);
    close(fd);
    free(c);
}

void revers_output(int fd) {
    char c;
    long size = lseek(fd, 0L, SEEK_END);
    for (int j = 0, pos = 0; j < size; ++j) {
        lseek(fd, --pos, SEEK_END);
        read(fd, &c, 1);
        printf("%c", c);
    }
    close(fd);
}
