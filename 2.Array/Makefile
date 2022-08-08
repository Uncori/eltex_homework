CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: array

array:
	@${CC} ${CFLAGS} main.c -o array

run:
	@./array

clean:
	@rm array

rebuild: clean array
