#include "func.h"

int check_lexeme(char number) {
    return (number >= '0' && number <= '9') || number == '.' ? SUCCESS : FAILURE;
}

int check_oper(char simbol) {
    int result = -2;
    if (simbol == COS || simbol == SIN || simbol == TAN || simbol == ACOS || simbol == ASIN ||
        simbol == ATAN || simbol == SQRT || simbol == LN || simbol == LOG || simbol == 'a') {
        result = FUNC_CHAR;
    } else if (simbol == '^') {
        result = MEDIUM_CHAR;
    } else if (simbol == '*' || simbol == '/' || simbol == '%') {
        result = EASY_CHAR;
    } else if (simbol == '+' || simbol == '-') {
        result = LOW_CHAR;
    } else {
        result = -2;
    }
    return result;
}

void check_func(st_c **stack_oper, char *output_str) {
    if ((*stack_oper)->simbol == 'c') {
        strncat(output_str, "cos", 4);
        strncat(output_str, " ", 2);
        pop_char(stack_oper);
    } else if ((*stack_oper)->simbol == 's') {
        strncat(output_str, "sin", 4);
        strncat(output_str, " ", 2);
        pop_char(stack_oper);
    } else if ((*stack_oper)->simbol == 't') {
        strncat(output_str, "tan", 4);
        strncat(output_str, " ", 2);
        pop_char(stack_oper);
    } else if ((*stack_oper)->simbol == 'C') {
        strncat(output_str, "acos", 5);
        strncat(output_str, " ", 2);
        pop_char(stack_oper);
    } else if ((*stack_oper)->simbol == 'S') {
        strncat(output_str, "asin", 5);
        strncat(output_str, " ", 2);
        pop_char(stack_oper);
    } else if ((*stack_oper)->simbol == 'T') {
        strncat(output_str, "atan", 5);
        strncat(output_str, " ", 2);
        pop_char(stack_oper);
    } else if ((*stack_oper)->simbol == 'q') {
        strncat(output_str, "sqrt", 5);
        strncat(output_str, " ", 2);
        pop_char(stack_oper);
    } else if ((*stack_oper)->simbol == 'l') {
        strncat(output_str, "ln", 3);
        strncat(output_str, " ", 2);
        pop_char(stack_oper);
    } else if ((*stack_oper)->simbol == 'L') {
        strncat(output_str, "log", 4);
        strncat(output_str, " ", 2);
        pop_char(stack_oper);
    } else {
        strncat(output_str, &(*stack_oper)->simbol, 1);
        strncat(output_str, " ", 2);
        pop_char(stack_oper);
    }
}

void check_func_cmp(st_c **stack_oper, char *output_str) {
    if (strcmp(output_str, "cos") == 0) {
        *stack_oper = push_char(*stack_oper, 'c');
    } else if (strcmp(output_str, "sin") == 0) {
        *stack_oper = push_char(*stack_oper, 's');
    } else if (strcmp(output_str, "tan") == 0) {
        *stack_oper = push_char(*stack_oper, 't');
    } else if (strcmp(output_str, "acos") == 0) {
        *stack_oper = push_char(*stack_oper, 'C');
    } else if (strcmp(output_str, "asin") == 0) {
        *stack_oper = push_char(*stack_oper, 'S');
    } else if (strcmp(output_str, "atan") == 0) {
        *stack_oper = push_char(*stack_oper, 'T');
    } else if (strcmp(output_str, "sqrt") == 0) {
        *stack_oper = push_char(*stack_oper, 'q');
    } else if (strcmp(output_str, "ln") == 0) {
        *stack_oper = push_char(*stack_oper, 'l');
    } else if (strcmp(output_str, "log") == 0) {
        *stack_oper = push_char(*stack_oper, 'L');
    }
}

int check_unar(char previous, char next) {
    int resoult = FAILURE;
    if (check_lexeme(next) == SUCCESS) {
        if (check_lexeme(previous) == FAILURE) {
            if (previous != ')') {
                resoult = SUCCESS;
            }
        }
    }
    return resoult;
}

int check_simbol(char simbol) {
    int error = FAILURE;
    if (simbol == '+' || simbol == '-' || simbol == '^' || simbol == '/' || simbol == '*') {
        error = SUCCESS;
    }
    return error;
}
void arithmetic_operations(st_d **stack_oper, char simbol) {
    if (simbol == '+') {
        double b = pop_double(stack_oper);
        double a = pop_double(stack_oper);
        *stack_oper = push_double(*stack_oper, a + b);
    } else if (simbol == '-') {
        double b = pop_double(stack_oper);
        double a = pop_double(stack_oper);
        *stack_oper = push_double(*stack_oper, a - b);
    } else if (simbol == '*') {
        double b = pop_double(stack_oper);
        double a = pop_double(stack_oper);
        *stack_oper = push_double(*stack_oper, a * b);
    } else if (simbol == '/') {
        double b = pop_double(stack_oper);
        double a = pop_double(stack_oper);
        if (b != 0) {
            *stack_oper = push_double(*stack_oper, a / b);
        } else {
            *stack_oper = push_double(*stack_oper, NAN);
        }
    } else if (simbol == '^') {
        double b = pop_double(stack_oper);
        double a = pop_double(stack_oper);
        *stack_oper = push_double(*stack_oper, pow(a, b));
    } else if (simbol == '%') {
        double b = pop_double(stack_oper);
        double a = pop_double(stack_oper);
        *stack_oper = push_double(*stack_oper, fmod(a, b));
    }
}

void arithmetic_operations_func(st_d **stack_oper, char *func) {
    if (strcmp(func, "cos") == 0) {
        double a = pop_double(stack_oper);
        *stack_oper = push_double(*stack_oper, cos(a));
    } else if (strcmp(func, "sin") == 0) {
        double a = pop_double(stack_oper);
        *stack_oper = push_double(*stack_oper, sin(a));
    } else if (strcmp(func, "tan") == 0) {
        double a = pop_double(stack_oper);
        *stack_oper = push_double(*stack_oper, tan(a));
    } else if (strcmp(func, "acos") == 0) {
        double a = pop_double(stack_oper);
        *stack_oper = push_double(*stack_oper, acos(a));
    } else if (strcmp(func, "asin") == 0) {
        double a = pop_double(stack_oper);
        *stack_oper = push_double(*stack_oper, asin(a));
    } else if (strcmp(func, "atan") == 0) {
        double a = pop_double(stack_oper);
        *stack_oper = push_double(*stack_oper, atan(a));
    } else if (strcmp(func, "sqrt") == 0) {
        double a = pop_double(stack_oper);
        *stack_oper = push_double(*stack_oper, sqrt(a));
    } else if (strcmp(func, "ln") == 0) {
        double a = pop_double(stack_oper);
        *stack_oper = push_double(*stack_oper, log(a));
    } else if (strcmp(func, "log") == 0) {
        double a = pop_double(stack_oper);
        *stack_oper = push_double(*stack_oper, log10(a));
    }
}
