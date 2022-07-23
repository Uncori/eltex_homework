CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: flow

flow:
	${CC} ${CFLAGS} main.c pipe.c -o pipe

run:
	./pipe
	
clean:
	rm -rf pipe