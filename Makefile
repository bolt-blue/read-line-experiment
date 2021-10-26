CFLAGS := -O0 -g -Wall -Wextra -Werror 
BUFSZ ?= 12

all: test

test: test.c read_line.h read_line.c read_line_utils.c
	${CC} ${CFLAGS} -D BUFSZ=${BUFSZ} read_line_utils.c read_line.c $< -o $@
