#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
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
      pid = waitpid(-1, &status, 0);
    } else if (pid == 0) {
      execvp(argv[1], arrayList);
    } else {
      perror("fork");
    }

    exit(EXIT_SUCCESS);
  } else {
    exit(EXIT_FAILURE);
    printf("not enough arguments\n");
  }

  return 0;
}

int pipeFd[2];
pipe(pipeFd);
int pid1, pid2;
pid1 = fork();
if (pid1 == 0) {
  dup2(pipeFd[1], STDOUT_FILENO);
  close(pipeFd[0]);
  close(pipeFd[1]);
  execlp("ls", "ls", "-la", NULL);
}
pid2 = fork();
if (pid2 == 0) {
  dup2(pipeFd[0], STDIN_FILENO);
  close(pipeFd[0]);
  close(pipeFd[1]);
  execlp("grep", "grep", "a.", NULL);
}

close(pipeFd[0]);
close(pipeFd[1]);

waitpid(pid1, NULL, 0);
waitpid(pid2, NULL, 0);

exit(EXIT_SUCCESS);