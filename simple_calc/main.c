#include "func.h"
#include "parsing.h"

int main() {
    char ex = ' ';
    while (ex != 'q') {
        char *str = (char *)calloc(MAX_LINE, sizeof(char));
        printf("Enter an expression(q - exit):\n");
        fgets(str, 255, stdin);
        ex = str[0];

        double res = parsing(str);
        printf("res = %lf\n", res);
        printf("---------------------------\n");

        free(str);
    }

    exit(0);
}
