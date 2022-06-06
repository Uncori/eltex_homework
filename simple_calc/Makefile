CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: simple_calc

simple_calc:
	${CC} ${CFLAGS} simple_calc.c main.c -o simple_calc ; ./simple_calc

clean:
	rm -rf simple_calc