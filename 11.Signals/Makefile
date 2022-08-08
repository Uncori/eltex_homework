CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: signal

signal:
	@${CC} ${CFLAGS} main.c -o signal

clean:
	@rm -rf signal

rebuild: clean signal