CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: sbash one two

sbash:
	@${CC} ${CFLAGS} bash.c -o sbash

one:
	@${CC} ${CFLAGS} one.c -o one

two:
	@${CC} ${CFLAGS} two.c -o two

clean:
	@rm -rf sbash one two

rebuild: clean sbash one two