CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: flow

flow:
	@${CC} ${CFLAGS} func.c main.c -o flow -lpthread

run:
	@./flow

rebuild: clean flow

clean:
	@rm -rf flow