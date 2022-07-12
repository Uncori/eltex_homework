CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: sbash

sbash:
	@${CC} ${CFLAGS} bash.c -o sbash

clean:
	@rm -rf sbash

rebuild: clean s21_cat