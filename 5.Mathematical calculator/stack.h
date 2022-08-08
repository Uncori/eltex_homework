#ifndef _STACK_H_
#define _STACK_H_

typedef struct stack_char {
    char simbol;
    struct stack_char *next;
} st_c;

st_c *push_char(st_c *ptr, char ham);
char pop_char(st_c **stack);

typedef struct stack_double {
    double number;
    struct stack_double *next;
} st_d;

st_d *push_double(st_d *ptr, double ham);
double pop_double(st_d **stack);

#endif  // _STACK_H_
