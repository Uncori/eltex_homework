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
    // pthread_exit(NULL);
}

static void *buyArray(void *arg) {
    int step = *((int *)arg);
    int rand_i = rand() % (3 - 0 + 1) + 0;

    while (arrayBuyer[step] > 0) {
        pthread_mutex_lock(&mutex);
        printf("pthread_self() = %ld\n", pthread_self());
        printf("arrayShop[%d] = %d\n", rand_i, arrayShop[rand_i]);
        printf("arrayBuyer[%d] = %d\n", step, arrayBuyer[step]);
        if (arrayShop[rand_i] > arrayBuyer[step]) {
            arrayShop[rand_i] -= arrayBuyer[step];
            arrayBuyer[step] -= arrayShop[rand_i];
        } else {
            arrayBuyer[step] -= arrayShop[rand_i];
            arrayShop[rand_i] = 0;
        }
        printf("   arrayShop[%d] = %d\n", rand_i, arrayShop[rand_i]);
        printf("   arrayBuyer[%d] = %d\n", step, arrayBuyer[step]);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}
void *fillShop(void *arg) {
    while (1) {
        for (int i = 0; i < SHOP_SIZE; i++) {
            pthread_mutex_lock(&mutex);
            arrayShop[i] += 500;
            pthread_mutex_unlock(&mutex);
            sleep(1);
        }
    }
}

int main(void) {
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

    s = pthread_create(&loaderFlow, NULL, fillShop, NULL);
    if (s != 0) {
        printf("error create\n");
    } else {
        printf("Message from main()\n");
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