#include <stdio.h>
#include <stdlib.h>

#include "read_line.h"

#define STDIN 0

int main(void)
{
    char *line;
    int line_count = 0;

    while ((line = read_line(STDIN)) != NULL) {
        printf("Line %02d:\n%s\n\n", ++line_count, line);
        free(line);
    }
    return 0;
}
