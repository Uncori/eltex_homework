#include <stdio.h>

#define N 5
void funcOne();
void funcTwo();
void funcFree();
void funcFour();

int main(void) {
    funcOne();
    funcTwo();
    funcFree();
    funcFour();
}

void funcOne() {
    int array[N][N];
    int k = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            array[i][j] = k;
            if (array[i][j] < 10) {
                printf(" %d ", array[i][j]);
            } else {
                printf("%d ", array[i][j]);
            }
            ++k;
        }
        printf("\n");
    }

    printf("-----------------\n");
}
void funcTwo() {
    int array_2[N];
    printf("---> ");
    for (int i = 0; i < N; ++i) {
        array_2[i] = i + 1;
        printf("%d ", array_2[i]);
    }
    printf("\n");

    printf("---> ");
    for (int i = N - 1; i >= 0; --i) {
        printf("%d ", array_2[i]);
    }
    printf("\n");

    printf("-----------------\n");
}
void funcFree() {
    int array[N][N];
    int temp = 4;
    for (int i = 0; i < N; ++i) {
        for (int j = N - 1; j >= temp; --j) {
            array[i][j] = 1;
        }
        --temp;
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < (N - 1) - i; ++j) {
            array[i][j] = 0;
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
    printf("-----------------\n");
}
void funcFour() {
    int array_sp[N][N], counter = 1, startCol = 0, endCol = N - 1, startRow = 0, endRow = N - 1;

    while (startCol <= endCol && startRow <= endRow) {
        for (int i = startCol; i <= endCol; ++i) {
            array_sp[startRow][i] = counter;
            ++counter;
        }
        ++startRow;

        for (int j = startRow; j <= endRow; ++j) {
            array_sp[j][endCol] = counter;
            ++counter;
        }
        --endCol;

        for (int k = endCol; k >= startCol; --k) {
            array_sp[endRow][k] = counter;
            ++counter;
        }
        --endRow;

        for (int temp = endRow; temp >= startRow; --temp) {
            array_sp[temp][startCol] = counter;
            ++counter;
        }
        ++startCol;
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%d ", array_sp[i][j]);
        }
        printf("\n");
    }
    printf("-----------------\n");
}
