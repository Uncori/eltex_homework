#include "func.h"

extern pthread_mutex_t mutex;

int main(void) {
    srand(time(NULL));
    pthread_t fillFLow;
    pthread_t buyerFlow[BUYERFLOW_COUNT];
    pthread_t loaderFlow;

    int result;
    void *res;

    result = pthread_mutex_init(&mutex, NULL);
    checkResultMutInit(&result);

    result = pthread_create(&fillFLow, NULL, fillArrays, NULL);
    checkResultCreate(&result);

    // result = pthread_cancel(fillFLow);
    // checkResultCancel(&result);

    result = pthread_join(fillFLow, NULL);
    checkResultJoin(&result);

    // if (res == PTHREAD_CANCELED) {
    //     printf("Thread was canceled\n");
    //     exit(EXIT_SUCCESS);
    // } else {
    //     printf("Thread was not canceled (should not happen!)\n");
    // }

    result = pthread_create(&loaderFlow, NULL, fillShop, NULL);
    checkResultCreate(&result);

    for (int i = 0; i < BUYERFLOW_COUNT; ++i) {
        result = pthread_create(&buyerFlow[i], NULL, buyArray, (void *)&i);
        checkResultCreate(&result);
        sleep(1);
    }

    for (int i = 0; i < BUYERFLOW_COUNT; ++i) {
        result = pthread_join(buyerFlow[i], NULL);
        checkResultJoin(&result);
    }
    result = pthread_join(loaderFlow, NULL);
    checkResultJoin(&result);

    pthread_mutex_destroy(&mutex);
    exit(EXIT_SUCCESS);
}