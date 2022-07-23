#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct data_struct {
  char **arrayParm;
  int size;
} Data;

int main(int argc, char *argv[]) {
  if (argc > 1) {
    int countPipe = 0;

    for (int i = 1; i < argc; ++i) {  // считываем кол-во '|'
      if (!strncmp(argv[i], "|", 2)) {
        countPipe += 1;
      }
    }

    int massPipe[countPipe];  // выделяем массив для индексов '|'
    int j = 0;
    for (int i = 1; i < argc; ++i) {  // заполняем массив индексами '|'
      if (!strncmp(argv[i], "|", 2)) {
        massPipe[j] = i;
        ++j;
      }
    }
    // выделяем память под аргументы
    Data *parmDate = (Data *)calloc(countPipe, sizeof(Data));
    for (int i = 0; i < countPipe; ++i) {
      parmDate[i].arrayParm = (char **)calloc(1, sizeof(char *));
      parmDate[i].size = massPipe[i];
    }
    for (int i = 0; i < countPipe; ++i) {
      for (int j = 0; j < massPipe[i]; ++j) {
        parmDate[i].arrayParm[j] = (char *)calloc(255, sizeof(char));
      }
    }

    int k = massPipe[0] + 1;
    for (int i = 0; i < countPipe; ++i) {
      for (int j = 0; j < parmDate[i].size; ++j) {
        for (; k < massPipe[i]; ++k) {
          parmDate[i].arrayParm[j] = argv[k];
          printf(" [%d]%s ", k, argv[k]);
        }
        printf("\n");
        printf("j\n");
      }

      printf("k\n");
    }
    /*for (int i = 0; i < countPipe; ++i) {
      for (int j = 0; j < parmDate[i].size; ++j) {
        printf("%s\n", parmDate[i].arrayParm[j]);
      }
    }*/

    /*char *arrayList[indexPipe];
    arrayList[indexPipe - 1] = NULL;
    for (int i = 1; i < indexPipe; ++i) {
      arrayList[i - 1] = argv[i];
    }
    printf("indexPipe = %d\n", indexPipe);
    for (int i = 0; i < indexPipe; ++i) {
      printf("%s ", arrayList[i]);
    }*/
  }
}