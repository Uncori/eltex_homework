CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: point

point:
	@${CC} ${CFLAGS} main.c -o point

run:
	@./point

clean:
	@rm point

rebuild: clean point
