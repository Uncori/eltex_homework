CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE
SIN_PORT = 5010
PID_SERVER := $(shell pidof -s server)

all: server client

server:
	@${CC} ${CFLAGS} server.c func.c -o server -lpthread

kill:
	@kill -15 ${PID_SERVER}

client:
	@${CC} ${CFLAGS} client.c func.c -o client

run_server:
	@./server

run_client:
	@sudo ./client

tel:
	@telnet localhost ${SIN_PORT}

rebuild: clean server client

valgrind_server:
	@valgrind --leak-check=full --tool=memcheck ./server

valgrind_client:
	@valgrind --leak-check=full --tool=memcheck ./client

clean:
	@rm -rf server client