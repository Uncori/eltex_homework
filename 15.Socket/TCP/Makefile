CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: server client

server:
	@${CC} ${CFLAGS} server.c -o server

client:
	@${CC} ${CFLAGS} client.c -o client

run_server:
	@./server

run_client:
	@./client

rebuild: clean server client

valgrind_server:
	@valgrind --leak-check=full --tool=memcheck ./server

valgrind_client:
	@valgrind --leak-check=full --tool=memcheck ./client

clean:
	@rm -rf server client