CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -D_GNU_SOURCE

all: build 

build:
	@${CC} ${CFLAGS} client.c -o client
	@${CC} ${CFLAGS} server.c -o server

run_server:
	@./server

clean:
	@rm -rf client server /tmp/server_test

rebuild: clean client server