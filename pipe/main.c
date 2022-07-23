#include "pipe.h"

int main(int argc, char *argv[]) {
  if (argc > 1) {
    int countPipe = -1, indexPipe = -1;
    checkArgv(&countPipe, &indexPipe, argc, argv);

    if (!countPipe && argc != indexPipe + 1) {
      workWithPipe(indexPipe, argc, argv);
    } else if (indexPipe && countPipe) {
      workWithOutPipe(argc, argv);
    } else {
      printf("syntax error\n");
      exit(EXIT_FAILURE);
    }
  } else {
    printf("not enough arguments\n");
    exit(EXIT_FAILURE);
  }
  return 0;
}