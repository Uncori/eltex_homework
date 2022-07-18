#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SHOP_SIZE 4
#define BUYERFLOW_COUNT 3

static int arrayShop[SHOP_SIZE];
static int arrayBuyer[BUYERFLOW_COUNT];
pthread_mutex_t mutex;

static void *fillArrays(void *arg) {
    for (int i = 0; i < SHOP_SIZE; ++i) {
        arrayShop[i] = rand() % (510 - 490 + 1) + 490;
    }
    for (int i = 0; i < BUYERFLOW_COUNT; ++i) {
        arrayBuyer[i] = 10000;
    }
    pthread_exit(NULL);
}
void logBuyer(int arrayShop_i, int arrayBuyer_i) {
    printf("\n");
    printf("-----------------Отчёт-магазина-----------------\n");
    printf("Покупатель: %d\n", arrayBuyer_i + 1);
    printf("Посетил магазин %d\n", arrayShop_i + 1);
    printf("Текущий бюджет: %d\n", arrayBuyer[arrayBuyer_i]);
    printf("------------------------------------------------\n");
    printf("\n");
}

void logLoader(int loaderFlow) {
    printf("\n");
    printf("------------------Отчёт-погрузчика------------------\n");
    printf("Погрузчик загрузил 500 товаров в %d-й магазин\n", loaderFlow);
    printf("Количество товаров в %d-ом магазине: %d\n", loaderFlow, arrayShop[loaderFlow]);
    printf("------------------------------------------------\n");
    printf("\n");
}

static void *buyArray(void *arg) {
    int step = *((int *)arg);
    int rand_i = 0;

    while (arrayBuyer[step] >= 0) {
        rand_i = rand() % (3 - 0 + 1) + 0;
        pthread_mutex_lock(&mutex);
        if (arrayShop[rand_i] > arrayBuyer[step]) {
            arrayShop[rand_i] -= arrayBuyer[step];
            arrayBuyer[step] -= arrayShop[rand_i];
        } else {
            arrayBuyer[step] -= arrayShop[rand_i];
            arrayShop[rand_i] = 0;
        }
        pthread_mutex_unlock(&mutex);
        logBuyer(rand_i, step);
        sleep(2);
    }
    return NULL;
}
void *fillShop(void *arg) {
    while (1) {
        for (int i = 0; i < SHOP_SIZE; ++i) {
            pthread_mutex_lock(&mutex);
            arrayShop[i] += 500;
            pthread_mutex_unlock(&mutex);
            logLoader(i);
            sleep(1);
        }
    }
    return NULL;
}

int main(void) {
    srand(time(NULL));
    pthread_t fillFLow;
    pthread_t buyerFlow[BUYERFLOW_COUNT];
    pthread_t loaderFlow;

    pthread_mutex_init(&mutex, NULL);

    int s;
    s = pthread_create(&fillFLow, NULL, fillArrays, NULL);
    if (s != 0) {
        printf("error create\n");
    } else {
        printf("Message from main()\n");
    }

    s = pthread_join(fillFLow, NULL);
    if (s != 0) {
        printf("error join\n");
    } else {
        printf("Thread fillFLow returned\n");
    }

    s = pthread_create(&loaderFlow, NULL, fillShop, NULL);
    if (s != 0) {
        printf("error create\n");
    } else {
        printf("Message from main()\n");
    }

    for (int i = 0; i < BUYERFLOW_COUNT; ++i) {
        s = pthread_create(&buyerFlow[i], NULL, buyArray, &i);
    }

    for (int i = 0; i < BUYERFLOW_COUNT; ++i) {
        s = pthread_join(buyerFlow[i], NULL);
        if (s != 0) {
            printf("error join\n");
        } else {
            printf("Thread buyerFlow[%d] returned\n", i);
        }
    }
    s = pthread_join(loaderFlow, NULL);
    if (s != 0) {
        printf("error join\n");
    } else {
        printf("Thread loaderFlow returned\n");
    }

    pthread_mutex_destroy(&mutex);
    exit(EXIT_SUCCESS);
}