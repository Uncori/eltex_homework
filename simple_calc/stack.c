#include "stack.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// char stack
st_c *s21_push_char(st_c *ptr, char ham) {
    st_c *temp = (st_c *)malloc(sizeof(st_c));
    temp->simbol = ham;
    temp->next = ptr;
    return temp;
}

char s21_pop_char(st_c **stack) {
    char simbol = ' ';
    if (*stack != NULL) {
        simbol = (*stack)->simbol;
        st_c *temp = (*stack);
        (*stack) = (*stack)->next;
        free(temp);
    }
    return simbol;
}
// double stack
st_d *s21_push_double(st_d *ptr, double ham) {
    st_d *temp = (st_d *)malloc(sizeof(st_d));
    temp->number = ham;
    temp->next = ptr;
    return temp;
}

double s21_pop_double(st_d **stack) {
    double number = NAN;
    if (*stack != NULL) {
        number = (*stack)->number;
        st_d *temp = (*stack);
        (*stack) = (*stack)->next;
        free(temp);
    }
    return number;
}
