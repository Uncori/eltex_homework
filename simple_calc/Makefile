CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: simple_calc

simple_calc:
	${CC} ${CFLAGS} -c simple_calc.c
	ar rc libfunc.a simple_calc.o
	ranlib libfunc.a
	${CC} ${CFLAGS} main.c -L. -lfunc -o calc


clean:
	rm calc *.o *.a