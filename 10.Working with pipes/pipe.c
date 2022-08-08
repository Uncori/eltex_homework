#include "pipe.h"

void checkArgv(int *countPipe, int *indexPipe, int argc, char *argv[]) {
  for (int i = 1; i < argc && *countPipe; ++i) {
    if (!strncmp(argv[i], "|", 2)) {
      *countPipe += 1;
      *indexPipe = i;
    }
  }
}

void allocateAraay(const int *indexPipe, int argc, char *firstArr[],
                   char *secondArr[], char *argv[]) {
  firstArr[*indexPipe - 1] = NULL;
  secondArr[argc - *indexPipe - 1] = NULL;
  for (int i = 1; i < *indexPipe; ++i) {
    firstArr[i - 1] = argv[i];
  }

  int j = 0;
  for (int i = *indexPipe + 1; i < argc; ++i) {
    secondArr[j] = argv[i];
    ++j;
  }
}

void checkRes(const int *res) {
  if (*res == -1) exit(EXIT_FAILURE);
}

void workWithPipe(int indexPipe, int argc, char *argv[]) {
  char *firstArr[indexPipe];
  char *secondArr[argc - indexPipe];
  allocateAraay(&indexPipe, argc, firstArr, secondArr, argv);

  int pipeFd[2], res = 0, pid1 = 0, pid2 = 0;

  res = pipe(pipeFd);
  checkRes(&res);

  pid1 = fork();
  checkRes(&pid1);

  if (!pid1) {
    res = dup2(pipeFd[1], STDOUT_FILENO);
    checkRes(&res);
    res = close(pipeFd[0]);
    checkRes(&res);
    res = close(pipeFd[1]);
    checkRes(&res);
    res = execvp(firstArr[0], firstArr);
    checkRes(&res);
  }

  pid2 = fork();
  checkRes(&pid1);

  if (!pid2) {
    res = dup2(pipeFd[0], STDIN_FILENO);
    checkRes(&res);
    res = close(pipeFd[0]);
    checkRes(&res);
    res = close(pipeFd[1]);
    checkRes(&res);
    res = execvp(secondArr[0], secondArr);
    checkRes(&res);
  }

  res = close(pipeFd[0]);
  checkRes(&res);
  res = close(pipeFd[1]);
  checkRes(&res);

  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);

  exit(EXIT_SUCCESS);
}

void workWithOutPipe(int argc, char *argv[]) {
  pid_t pid, res = 0;
  char *arrayList[argc];
  arrayList[argc - 1] = NULL;

  for (int i = 1; i < argc; ++i) {
    arrayList[i - 1] = argv[i];
  }

  pid = fork();
  checkRes(&pid);
  waitpid(-1, NULL, 0);
  if (!pid) {
    res = execvp(argv[1], arrayList);
    checkRes(&res);
  }

  exit(EXIT_SUCCESS);
}
