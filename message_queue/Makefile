CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: server

server:
	@${CC} ${CFLAGS} server.c -o server -lrt

run:
	@./server

rebuild: clean server run
	
clean:
	@rm -rf server