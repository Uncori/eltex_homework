CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: bit

bit:
	@${CC} ${CFLAGS} main.c -o bit

run:
	@./bit

clean:
	@rm bit

rebuild: clean bit
