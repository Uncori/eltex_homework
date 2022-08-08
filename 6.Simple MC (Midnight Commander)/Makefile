CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: mc

mc:
	@${CC} ${CFLAGS} main.c function.c -lncurses -o mc

run:
	@./mc

rebuild: clean mc

clean:
	rm mc
