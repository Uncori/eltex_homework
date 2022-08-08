#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid;
    int status = 0;
    pid = fork();
    if (pid) {
        printf("PARENT: Это процесс-родитель!\n");
        printf("PARENT: Мой PID -- %d\n", getpid());
        printf("PARENT: PID моего родителя -- %d\n", getppid());
        printf("PARENT: PID моего потомка %d\n\n", pid);
        waitpid(-1, &status, 0);
        pid = fork();
        if (pid) {
            printf("PARENT: Это процесс-родитель!\n");
            printf("PARENT: Мой PID -- %d\n", getpid());
            printf("PARENT: PID моего родителя -- %d\n", getppid());
            printf("PARENT: PID моего потомка %d\n\n", pid);
            waitpid(-1, &status, 0);
        } else if (pid == 0) {
            printf(" CHILD: Это процесс-потомок!\n");
            printf(" CHILD: Мой PID -- %d\n", getpid());
            printf(" CHILD: PID моего родителя -- %d\n\n", getppid());
            pid = fork();
            if (pid) {
                printf("PARENT: Это процесс-родитель!\n");
                printf("PARENT: Мой PID -- %d\n", getpid());
                printf("PARENT: PID моего родителя -- %d\n", getppid());
                printf("PARENT: PID моего потомка %d\n\n", pid);
                waitpid(-1, &status, 0);
                pid = fork();
                if (pid) {
                    printf("PARENT: Это процесс-родитель!\n");
                    printf("PARENT: Мой PID -- %d\n", getpid());
                    printf("PARENT: PID моего родителя -- %d\n", getppid());
                    printf("PARENT: PID моего потомка %d\n\n", pid);
                    waitpid(-1, &status, 0);
                } else if (pid == 0) {
                    printf(" CHILD: Это процесс-потомок!\n");
                    printf(" CHILD: Мой PID -- %d\n", getpid());
                    printf(" CHILD: PID моего родителя -- %d\n\n", getppid());
                } else {
                    perror("fork");
                }
            } else if (pid == 0) {
                printf(" CHILD: Это процесс-потомок!\n");
                printf(" CHILD: Мой PID -- %d\n", getpid());
                printf(" CHILD: PID моего родителя -- %d\n\n", getppid());
            } else {
                perror("fork");
            }

        } else {
            perror("fork");
        }
    } else if (pid == 0) {
        printf(" CHILD: Это процесс-потомок!\n");
        printf(" CHILD: Мой PID -- %d\n", getpid());
        printf(" CHILD: PID моего родителя -- %d\n\n", getppid());
        pid = fork();
        if (pid) {
            printf("PARENT: Это процесс-родитель!\n");
            printf("PARENT: Мой PID -- %d\n", getpid());
            printf("PARENT: PID моего родителя -- %d\n", getppid());
            printf("PARENT: PID моего потомка %d\n\n", pid);
            waitpid(-1, &status, 0);
        } else if (pid == 0) {
            printf(" CHILD: Это процесс-потомок!\n");
            printf(" CHILD: Мой PID -- %d\n", getpid());
            printf(" CHILD: PID моего родителя -- %d\n\n", getppid());
        } else {
            perror("fork");
        }

    } else {
        perror("fork");
    }
}