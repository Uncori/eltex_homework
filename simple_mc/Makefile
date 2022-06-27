CC = gcc
CFLAGS = -Wall -Werror -Wextra

all: mc

mc:
	${CC} ${CFLAGS} main.c function.c -lncurses -o mc ; ./mc

clean:
	rm mc
