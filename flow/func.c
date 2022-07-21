#include "func.h"

pthread_mutex_t mutex;

int arrayShop[SHOP_SIZE];
int arrayBuyer[BUYERFLOW_COUNT];
int flag = -1;

void *buyArray(void *arg) {
    int step = *((int *)arg);
    int rand_i = 0;

    while (arrayBuyer[step] > 0) {
        pthread_mutex_lock(&mutex);
        rand_i = rand() % ((SHOP_SIZE - 1) - 0 + 1) + 0;
        logBuyerBefore(rand_i, step);
        if (arrayShop[rand_i] > arrayBuyer[step]) {
            arrayShop[rand_i] -= arrayBuyer[step];
            arrayBuyer[step] = 0;
        } else {
            arrayBuyer[step] -= arrayShop[rand_i];
            arrayShop[rand_i] = 0;
        }
        logBuyerAfter(rand_i, step);
        pthread_mutex_unlock(&mutex);
        sleep(3);
    }
    flag = 0;
    pthread_exit(NULL);
}

void *fillArrays(void *arg) {
    if (arg) {
    }
    for (int i = 0; i < SHOP_SIZE; ++i) {
        arrayShop[i] = rand() % (510 - 490 + 1) + 490;
    }
    printf("-----------\n");
    for (int i = 0; i < BUYERFLOW_COUNT; ++i) {
        arrayBuyer[i] = BALANCE_BUYER;
    }
    pthread_exit(NULL);
}

void logBuyerBefore(int arrayShop_i, int arrayBuyer_i) {
    printf("\n");
    printf("-----------------Отчёт-магазина-----------------\n");
    printf("Покупатель: %d\n", arrayBuyer_i);
    printf("Посетил магазин %d\n", arrayShop_i);
    printf("Текущий бюджет покупателя: %d\n", arrayBuyer[arrayBuyer_i]);
    printf("Текущая касса магазина: %d\n", arrayShop[arrayShop_i]);
}

void logBuyerAfter(int arrayShop_i, int arrayBuyer_i) {
    printf("\tБюджет покупателя после покупок [%d]: %d\n", arrayBuyer_i, arrayBuyer[arrayBuyer_i]);
    printf("\tКасса магазина[%d] = %d\n", arrayShop_i, arrayShop[arrayShop_i]);
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

void *fillShop(void *arg) {
    if (arg) {
    }
    while (flag) {
        for (int i = 0; i < SHOP_SIZE; ++i) {
            pthread_mutex_lock(&mutex);
            arrayShop[i] += 500;
            logLoader(i);
            pthread_mutex_unlock(&mutex);
            sleep(1);
        }
    }
    pthread_exit(NULL);
}

void checkResultMutInit(int *result) {
    if (*result != 0) {
        perror("Multex initialization failed!\n");
        exit(EXIT_FAILURE);
    }
}

void checkResultCreate(int *result) {
    if (*result != 0) {
        perror("Thread creation failed!\n");
        exit(EXIT_FAILURE);
    }
}

void checkResultCancel(int *result) {
    if (*result != 0) {
        perror("Thread cancel failed!\n");
        exit(EXIT_FAILURE);
    }
}

void checkResultJoin(int *result) {
    if (*result != 0) {
        perror("Thread join failed!\n");
        exit(EXIT_FAILURE);
    }
}
