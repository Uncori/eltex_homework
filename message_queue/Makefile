CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: server client

server:
	@${CC} ${CFLAGS} server.c -o server -lrt

client:
	@${CC} ${CFLAGS} client.c -o client -lrt


run_server:
	@./server

run_client:
	@./client

rebuild: clean server client

test_server:
	@gcc server.c -o server -lrt

test_client:
	@gcc client.c -o client -lrt
	
clean:
	@rm -rf server client