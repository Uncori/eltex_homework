#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid;
    int exit_code = 0;
    pid = fork();
    if (pid) {
        printf("PARENT: Это процесс-родитель!\n");
        printf("PARENT: Мой PID -- %d\n", getpid());
        printf("PARENT: PID моего родителя -- %d\n", getppid());
        printf("PARENT: PID моего потомка %d\n", pid);
        printf("PARENT: Я жду, пока потомок не вызовет exit()...\n");
        exit_code = 0;
    } else if (pid == 0) {
        printf(" CHILD: Это процесс-потомок!\n");
        printf(" CHILD: Мой PID -- %d\n", getpid());
        printf(" CHILD: PID моего родителя -- %d\n", getppid());
        exit_code = 18;
    } else {
        perror("fork");
    }
    if (pid != 0) {
        int stat_val = 0;
        pid_t child_pid = 0;
        child_pid = wait(&stat_val);
        printf("child has finish pid = %d\n", child_pid);
        if (WIFEXITED(stat_val)) {
            printf("child %d\n", WEXITSTATUS(stat_val));
        } else {
            printf("ok\n");
        }
    }
    exit(exit_code);
}