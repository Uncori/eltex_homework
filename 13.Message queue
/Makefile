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

valgrind_server:
	@valgrind --leak-check=full --tool=memcheck ./server

valgrind_client:
	@valgrind --leak-check=full --tool=memcheck ./client

git:
	@git add --all
	@git commit -m "change" 
	@git push origin main
	
clean:
	@rm -rf server client