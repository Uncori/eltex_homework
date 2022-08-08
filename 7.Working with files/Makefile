CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: ww_files

ww_files:
	${CC} ${CFLAGS} functions.c main.c -o ww_files ; ./ww_files

clean:
	rm -rf ww_files