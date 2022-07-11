#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    char *arrayList[argc];
    arrayList[argc - 1] = NULL;

    if (argc >= 2) {
        int j = 0;
        for (int i = 1; i < argc; ++i) {
            arrayList[j] = argv[i];
            ++j;
        }
    }

    execvp(argv[1], arrayList);
    free(arrayList);
    exit(EXIT_SUCCESS);
}