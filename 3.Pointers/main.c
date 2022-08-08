#include <stdio.h>

int main(void) {
    char a = 'A';
    char b = 'B';

    char *ptr1 = &a;
    char **ptr2 = &ptr1;

    *ptr2 = &b;

    printf("a = %p\n", &a);
    printf("b = %p\n", &b);

    printf("ptr = %p\n", ptr1);
    printf("*ptr = %c\n", *ptr1);

    printf("---------------\n");

    int c = 1234567890;
    printf("c = %x\n", c);
    char *ptr = (char *)&c;

    for (size_t i = 0; i < 4; i++) {
        printf("ptr = %x\n", *ptr);
        ++ptr;
    }
}
