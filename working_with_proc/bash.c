#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    if (argc > 1) {
        pid_t pid;
        int status = 0;
        char *arrayList[argc];
        arrayList[argc - 1] = NULL;

        int j = 0;
        for (int i = 1; i < argc; ++i) {
            arrayList[j] = argv[i];
            ++j;
        }

        pid = fork();
        if (pid) {
            waitpid(-1, &status, 0);
        } else if (pid == 0) {
            execvp(argv[1], arrayList);
        } else {
            perror("fork");
        }

        exit(EXIT_SUCCESS);
    } else {
        printf("not enough arguments\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}