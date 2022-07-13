#ifndef SRC_S21_STACK_H_
#define SRC_S21_STACK_H_
// stack char
typedef struct stack_char {
    char simbol;
    struct stack_char *next;
} st_c;

st_c *s21_push_char(st_c *ptr, char ham);
char s21_pop_char(st_c **stack);

// stack double
typedef struct stack_double {
    double number;
    struct stack_double *next;
} st_d;

st_d *s21_push_double(st_d *ptr, double ham);
double s21_pop_double(st_d **stack);
#endif  // SRC_S21_STACK_H_
