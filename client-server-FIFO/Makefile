CC = gcc
CFLAGS = -std=c11 -D_GNU_SOURCE

all: build 

build:
	@${CC} ${CFLAGS} client.c -o client
	@${CC} ${CFLAGS} server.c -o server

client:
	@./client

server:
	@./server

clean:
	@rm -rf client server

rebuild: clean client server