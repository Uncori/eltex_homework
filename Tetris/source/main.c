#include <stdlib.h>

#include "../header/handler.h"

int ip_written = 0;

int main(int argc, char *argv[])
{
    if (argc > 0) {
        menu_handler(argv[1]);
    }

    exit(EXIT_SUCCESS);
}
