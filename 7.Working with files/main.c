#include "functions.h"

int main(void) {
    char *msg = "Hello world!";

    write_print(open(FILE_NAME, O_RDWR | O_TRUNC, 0), msg);
    revers_output(open(FILE_NAME, O_RDONLY, 0));
}
