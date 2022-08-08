CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: file

file:
	@${CC} ${CFLAGS} functions.c main.c -o file

run:
	@./file

rebuild: clean file

clean:
	@rm -rf file