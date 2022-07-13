CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE -lm

all: simple_calc

simple_calc:
	@${CC} ${CFLAGS} -c parsing.c func.c stack.c
	@ar rc libfunc.a parsing.o func.o stack.o
	@ranlib libfunc.a
	@${CC} ${CFLAGS} main.c -L. -lfunc -o calc
	@./calc


clean:
	@rm calc *.o *.a

rebuild: clean simple_calc