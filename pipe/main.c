#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int pipeFd[2];
  if (pipe(pipeFd) == -1) {
    exit(EXIT_FAILURE);
  }
  switch (fork()) {
    case -1:
      exit(EXIT_FAILURE);
    case 0:
      if (close(pipeFd[0]) == -1) {
        exit(EXIT_FAILURE);
      }
      if (pipeFd[1] != STDOUT_FILENO) {
        if (dup2(pipeFd[1], STDOUT_FILENO) == -1) {
          exit(EXIT_FAILURE);
        }
        if (close(pipeFd[1]) == -1) {
          exit(EXIT_FAILURE);
        }
      }
      execlp("ls", "ls", (char *)NULL);
    default:
      break;
  }

  switch (fork()) {
    case -1:
      exit(EXIT_FAILURE);
    case 0:
      if (close(pipeFd[1]) == -1) {
        exit(EXIT_FAILURE);
      }
      if (pipeFd[0] != STDOUT_FILENO) {
        if (dup2(pipeFd[0], STDOUT_FILENO) == -1) {
          exit(EXIT_FAILURE);
        }
        if (close(pipeFd[0]) == -1) {
          exit(EXIT_FAILURE);
        }
      }
      execlp("wc", "wc", "-l", (char *)NULL);
    default:
      break;
  }
  if (close(pipeFd[0]) == -1) {
    exit(EXIT_FAILURE);
  }
  if (close(pipeFd[1]) == -1) {
    exit(EXIT_FAILURE);
  }
  if (wait(NULL) == -1) {
    exit(EXIT_FAILURE);
  }
  if (wait(NULL) == -1) {
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}