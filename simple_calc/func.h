#ifndef SRC_S21_CALC_H_
#define SRC_S21_CALC_H_
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

#define SUCCESS 1
#define FAILURE 0
#define MAX_LINE 255
#define COS 'c'
#define SIN 's'
#define TAN 't'
#define ACOS 'C'
#define ASIN 'S'
#define ATAN 'T'
#define SQRT 'q'
#define LN 'l'
#define LOG 'L'

typedef enum {
    VERY_LOW_CHAR = -1,
    LOW_CHAR = 0,
    EASY_CHAR = 1,
    MEDIUM_CHAR = 2,
    FUNC_CHAR = 3
} char_type;

// help func
int s21_check_lexeme(char number);
int s21_check_oper(char simbol);
void s21_check_func(st_c **stack_oper, char *output_str);
void s21_check_func_cmp(st_c **stack_oper, char *output_str);
int s21_check_unar(char previous, char next);
int s21_check_simbol(char simbol);
void s21_arithmetic_operations(st_d **stack_oper, char simbol);
void s21_arithmetic_operations_func(st_d **stack_oper, char *func);
#endif  // SRC_S21_CALC_H_
