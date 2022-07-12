#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    if (argc > 1) {
        pid_t pid;
        int status = 0;
        char *arrayList[argc];

        arrayList[argc - 1] = NULL;

        if (argc >= 2) {
            int j = 0;
            for (int i = 1; i < argc; ++i) {
                arrayList[j] = argv[i];
                ++j;
            }
        }

        pid = fork();
        if (pid) {
            pid_t child_pid = 0;
            child_pid = waitpid(-1, &status, 0);
            printf("\nchild has finish pid = %d\n", child_pid);
        } else if (pid == 0) {
            execvp(argv[1], arrayList);
        } else {
            perror("fork");
        }

        exit(EXIT_SUCCESS);
    }
    printf("not enough arguments\n");
    return 0;
}