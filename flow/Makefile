CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: ww_files

ww_files:
	${CC} ${CFLAGS} func.c main.c -o flow -lpthread

clean:
	rm -rf flow