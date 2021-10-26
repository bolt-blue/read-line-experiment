#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "read_line.h"

// May be defined at compile time
#ifndef BUFSZ
#define BUFSZ 8
#endif
#define LINEBUF_BASE_SZ (512 > BUFSZ ? 512 : BUFSZ)

static int find_newline_(char *, int);

char *
read_line(int fd)
{
    static struct line_t line_buf;
    char buf[BUFSZ + 1] = {0};

    if (!line_buf.p) {
        line_buf.p = malloc_z(1, LINEBUF_BASE_SZ + 1);
        line_buf.sz = LINEBUF_BASE_SZ;
    }

    if (line_buf.len) {
        // Handle edge case of more than one newline
        // having been pulled into the buffer on last read
        int nl;
        if ((nl = find_newline_(line_buf.p, line_buf.len)) > -1) {
            line_buf.p[nl] = '\0';
            line_buf.len = nl;
            goto OUT;
        }
    }

    int bytes_read;

    while ((bytes_read = read(fd, buf, BUFSZ)) > 0) {
        // Keep buffer clean
        zero(buf + bytes_read, BUFSZ - bytes_read);

        // Allocate more space to the line buffer if necessary
        if (line_buf.sz - line_buf.len < bytes_read) {
            // WARN: Only safe if LINEBUF_BASE_SZ >= BUFSZ (see declaration)
            int new_sz = line_buf.sz + LINEBUF_BASE_SZ;

            char *tmp = malloc(new_sz + 1);
            str_cpy(tmp, line_buf.p);
            free(line_buf.p);
            line_buf.p = tmp;

            zero(line_buf.p + line_buf.sz, new_sz + 1);
            line_buf.sz = new_sz;
        }

        str_cat(line_buf.p + line_buf.len, buf);

        // Find first occurence of newline in buffer
        int nl;
        if ((nl = find_newline_(buf, BUFSZ)) > -1) {
            line_buf.len += nl;
            line_buf.p[line_buf.len] = '\0';
            break;
        } else {
            line_buf.len += bytes_read;
        }
    }

    if (bytes_read < 1)
        return NULL;

OUT:;
    char *out_str = malloc(line_buf.len + 1);
    str_cpy(out_str, line_buf.p);

    // Shift any data following the newly read line
    // to the front of the buffer, ready for next call
    if (line_buf.len < line_buf.sz) {
        // The 1 is so we skip the null terminator
        char *tail = line_buf.p + line_buf.len + 1;
        int tail_len = str_len(tail);

        mem_move(line_buf.p, tail);

        // Zero out only what's necessary
        {
            int old_len = line_buf.len;
            line_buf.len = tail_len;

            zero(line_buf.p + line_buf.len + 1, old_len);
        }

    } else {
        // Reset
        line_buf.len = 0;
    }

    return out_str;
}

int
find_newline_(char *s, int sz)
{
    for (int i = 0; i < sz; i++) {
        if (s[i] == '\n')
            return i;
    }
    return -1;
}
