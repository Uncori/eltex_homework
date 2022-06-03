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
    printf("*ptr = %c", *ptr1);
}
