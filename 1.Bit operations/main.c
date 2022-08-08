#include <stdio.h>

void print_byte(int *ptr) {
    printf("number = 0x%x\n", *ptr);
    unsigned int b = 0x000000FF;
    for (int i = 0, j = 0; i < 4; ++i, j += 8) {
        printf("%d byte = 0x%x\n", i + 1, (*ptr & b) >> j);
        b <<= 8;
    }
    printf("default number = 0x%x, change 3 byte = 0x%x\n", *ptr, (*ptr & 0x0000FFFF) | 0x00DD0000);
}

int main(void) {
    int a = 0x00CCAABB;
    print_byte(&a);
    return 0;
}