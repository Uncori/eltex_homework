CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: directory

directory:
	@${CC} ${CFLAGS} functions.c main.c -o directory

run:
	@./directory

rebuild: clen directory

clean:
	@rm -rf directory *.txt
