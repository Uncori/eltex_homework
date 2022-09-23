SRCS = $(wildcard source/*.c)
OBJS = $(SRCS:source/%.c=obj/%.o)
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LFlAGS = -lm -lncurses -pthread
LIB = lib/tetris_lib.a

all: obj lib tetris 

obj:
	@mkdir obj

lib:
	@mkdir lib

obj/%.o: source/%.c
	@${CC} ${CFLAGS} -c -o $@ $<

${LIB}: ${SRCS} ${OBJS} 
	@ar rcs ${LIB} ${OBJS}
	@ranlib ${LIB}

tetris: ${LIB}
	@${CC} ${CFLAGS} ${LIB} -o tetris ${LFlAGS}

gdb: clean ${LIB} 
	@${CC} ${CFLAGS} ${LIB} -o tetris ${LFlAGS} -g3
	@sudo gdb ./tetris

run:
	@sudo ./tetris

rebuild: clean tetris 

valgrind_t:
	@valgrind --leak-check=full --tool=memcheck --log-file=stdout.txt ./tetris

cppcheck:
	@cppcheck --enable=all --std=c11 --inconclusive --suppress=missingIncludeSystem source/*.c

clean:
	@rm -rf obj lib tetris resource/err_out.txt
