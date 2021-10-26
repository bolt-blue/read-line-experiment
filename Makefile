CFLAGS := -O0 -g -Wall -Wextra -Werror 

all: test

test: test.c read_line.h read_line.c read_line_utils.c
	${CC} ${CFLAGS} -D BUFSZ=12 read_line_utils.c read_line.c $< -o $@
