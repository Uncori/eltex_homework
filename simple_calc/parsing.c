#include "func.h"

double parsing(char *argv) {
    char *check_str = (char *)calloc(MAX_LINE, sizeof(char));
    for (size_t i = 0; i < strlen(argv) - 1; ++i) {
        strncat(check_str, &argv[i], 1);
    }
    char *output_str = (char *)calloc(MAX_LINE, sizeof(char));
    st_c *stack_oper = NULL;
    int error = SUCCESS;
    double resoult = NAN;
    if (check_str[0] == '*' || check_str[0] == '/' || check_str[0] == '%' || check_str[0] == '^') {
        error = FAILURE;
    } else {
        int count_bracket_open = 0;
        int count_dot = 0;
        int unar_type = FAILURE;
        for (size_t i = 0; i < strlen(check_str) && error; ++i) {
            if ((s21_check_lexeme(check_str[i]) == SUCCESS) && error) {
                int a = 0;
                int boba = 0;
                char *array_number = (char *)calloc(MAX_LINE, sizeof(char));
                for (a = i; s21_check_lexeme(check_str[a]) != FAILURE && error; a++) {
                    if (count_dot > 1) {
                        error = FAILURE;
                    } else {
                        array_number[boba] = check_str[a];
                        ++boba;
                    }
                    if (check_str[a] == '.' && error) {
                        count_dot++;
                    }
                }
                if (unar_type == SUCCESS) {
                    strncat(output_str, "0", 2);
                    strncat(output_str, " ", 2);
                    unar_type = FAILURE;
                }
                if (error) {
                    strncat(output_str, array_number, strlen(array_number));
                    strncat(output_str, " ", 2);
                    free(array_number);
                    count_dot = 0;
                    boba = 0;
                    i = a - 1;
                }
            } else if (check_str[i] == '(' && error) {
                stack_oper = s21_push_char(stack_oper, check_str[i]);
                count_bracket_open++;
            } else if (s21_check_oper(check_str[i]) == 3 && error) {
                if (s21_check_oper(check_str[i - 1]) == 0 && check_str[i - 2] == '(') {
                    strncat(output_str, "0", 2);
                    strncat(output_str, " ", 2);
                } else if (s21_check_oper(check_str[0]) == 0) {
                    strncat(output_str, "0", 2);
                    strncat(output_str, " ", 2);
                }
                char *array_func = (char *)calloc(MAX_LINE, sizeof(char));
                int biba = 0, b = 0;
                for (b = i; check_str[b] != '(' && error; ++b) {
                    if (check_str[b] == '\0') {
                        error = FAILURE;
                    }
                    array_func[biba] = check_str[b];
                    ++biba;
                }
                s21_check_func_cmp(&stack_oper, array_func);
                i = b - 1;
                free(array_func);
            } else if ((s21_check_oper(check_str[i]) >= -1 && s21_check_oper(check_str[i]) <= 3) &&
                       error) {
                if (s21_check_oper(check_str[i]) != -2 && s21_check_oper(check_str[i - 1]) != -2) {
                    error = FAILURE;
                } else {
                    if (s21_check_oper(check_str[i]) == 0) {
                        if (s21_check_unar(check_str[i - 1], check_str[i + 1]) == SUCCESS) {
                            unar_type = SUCCESS;
                        }
                    }
                    int nan = SUCCESS;
                    while (stack_oper != NULL && nan) {
                        if (s21_check_oper(stack_oper->simbol) >= s21_check_oper(check_str[i])) {
                            s21_check_func(&stack_oper, output_str);
                        } else {
                            nan = FAILURE;
                        }
                    }
                    stack_oper = s21_push_char(stack_oper, check_str[i]);
                }

            } else if (check_str[i] == ')' && error && count_bracket_open > 0) {
                while (stack_oper->simbol != '(') {
                    s21_check_func(&stack_oper, output_str);
                }
                s21_pop_char(&stack_oper);
                count_bracket_open--;
            } else {
                error = FAILURE;
            }
        }

        while (stack_oper != NULL && error) {
            if (stack_oper->simbol == '(') {
                error = FAILURE;
            } else {
                s21_check_func(&stack_oper, output_str);
            }
        }

        while (stack_oper != NULL && error) {
            s21_pop_char(&stack_oper);
        }
    }
    if (error) {
        st_d *output_stack = NULL;
        int count_number = 0;
        for (size_t i = 0; i < strlen(output_str); ++i) {
            int pup = 0;
            int j = 0;
            char *array_number = (char *)calloc(MAX_LINE, sizeof(char));
            for (j = i; output_str[j] != ' '; ++j) {
                array_number[pup] = output_str[j];
                pup++;
            }
            pup = 0;
            i = j;
            double number = 0.0;
            char simbol;
            sscanf(array_number, "%c", &simbol);
            if (sscanf(array_number, "%lf", &number) != 0) {
                output_stack = s21_push_double(output_stack, number);
                count_number++;
            }
            s21_arithmetic_operations_func(&output_stack, array_number);
            if (s21_check_simbol(simbol)) {
                if (count_number >= 2) {
                    s21_arithmetic_operations(&output_stack, simbol);
                    count_number -= 1;
                }
            }
            free(array_number);
        }
        resoult = s21_pop_double(&output_stack);
        free(output_str);
    } else {
        free(output_str);
    }
    free(check_str);
    return resoult;
}
