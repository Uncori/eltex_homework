#ifndef _FUNC_H_
#define _FUNC_H_

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SHOP_SIZE 4
#define BUYERFLOW_COUNT 3
#define BALANCE_BUYER 10000

void *fillArrays(void *arg);
void *buyArray(void *arg);
void logBuyerBefore(int arrayShop_i, int arrayBuyer_i);
void logBuyerAfter(int arrayShop_i, int arrayBuyer_i);
void logLoader(int loaderFlow);
void *fillShop(void *arg);
void checkResultMutInit(const int *result);
void checkResultCreate(const int *result);
void checkResultJoin(const int *result);

#endif  // _FUNC_H_
