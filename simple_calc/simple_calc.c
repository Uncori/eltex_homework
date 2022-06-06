#include "simple_calc.h"

void display_menu() {
    double a = 0.0, b = 0.0;
    char action = ' ';

    while (action != 'q') {
        printf("-------------------Simple_Calculator--------------------\n");
        printf("Enter two number\n");
        printf("fist number : ");
        scanf("%lf", &a);
        setbuf(stdin, NULL);
        printf("second number : ");
        scanf("%lf", &b);
        setbuf(stdin, NULL);
        printf("enter action ( +, -, *, /, q - exit) : ");
        action = getchar();
        setbuf(stdin, NULL);
        if (action != 'q') {
            double res = 0.0;
            printf("result = %lf\n", res = action_processing(&a, &b, &action));
        }
    }
}

double action_processing(double *a, double *b, char *c) {
    double res = 0.0;
    switch (*c) {
        case '+':
            res = sum_numbers(&a, &b);
            break;
        case '-':
            res = sub_numbers(&a, &b);
            break;
        case '*':
            res = mult_numbers(&a, &b);
            break;
        case '/':
            res = div_numbers(&a, &b);
            break;
        default:
            printf("syntax error\n");
            break;
    }
    return res;
}

double sum_numbers(double **a, double **b) { return **a + **b; }
double sub_numbers(double **a, double **b) { return **a - **b; }
double mult_numbers(double **a, double **b) { return **a * **b; }
double div_numbers(double **a, double **b) { return **a / **b; }
