#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void checkRes(const int *res) {
  if (*res == -1) exit(EXIT_FAILURE);
}

int main() {
  __pid_t pid;
  sigset_t set;
  int ret_val = 0, sig = 0;
  int *sigptr = &sig;
  ret_val = sigemptyset(&set);
  checkRes(&ret_val);
  ret_val = sigaddset(&set, SIGUSR1);
  checkRes(&ret_val);
  ret_val = sigprocmask(SIG_BLOCK, &set, NULL);
  checkRes(&ret_val);

  pid = fork();
  checkRes(&pid);

  if (pid) {
    while (1) {
      ret_val = sigwait(&set, sigptr);
      if (ret_val == -1)
        perror("sigwait failed\n");
      else {
        if (*sigptr == 10)
          printf("SIGUSR1 was received\n");
        else
          printf("sigwait returned with sig: %d\n", *sigptr);
      }
    }
  } else if (!pid) {
    while (1) {
      kill(getppid(), SIGUSR1);
      sleep(1);
    }

  } else {
    exit(EXIT_FAILURE);
  }
  waitpid(-1, NULL, 0);

  return 0;
}