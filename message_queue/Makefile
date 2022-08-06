CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: server

server:
	@${CC} ${CFLAGS} server.c -o server -lrt

run:
	@./server

rebuild: clean server run

test_server:
	@gcc server.c -o server -lrt

test_client:
	@gcc client.c -o client -lrt
	
clean:
	@rm -rf server