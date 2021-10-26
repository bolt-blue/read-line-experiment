#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "read_line.h"

/*
 * BUFSZ is the read buffer size - it may be defined at compile time
 */
#ifndef BUFSZ
#define BUFSZ 8
#endif
/*
 * LINEBUF_BASE_SZ is a dynamic scratch buffer that allows us to concatenate
 * lines that are longer than the read buffer.
 * Any enlargement of this buffer will be in multiples of this size.
 * We make sure this is larger than BUFSZ at compile time, to avoid
 * wasted effort at runtime.
 * TODO: Confirm the compiler optimises away the following ternary op.
 */
#define LINEBUF_BASE_SZ (512 > BUFSZ ? 512 : BUFSZ)

/* ========================================================================== */

struct line_t {
    char *buf;  // Line buffer
    int len;    // Length of string stored at `buf`, not including '\0'
    int sz;     // Allocated size in bytes
};

static int find_newline_(char *, int);

/* ========================================================================== */

/*
 * Read one line from an open file with descriptor `fd`
 * This is a buffered read, where each subsequent call will return
 * the next line.
 * The newline char is not preserved.
 * Returns:
 * - Pointer to malloc'd string on success
 *   The returned pointer must be free'd manually by the caller
 * - NULL pointer on EOF or failure
 * TODO: Catch and handle more error conditions
 */
char *
read_line(int fd)
{
    // TODO: How do we have the `line` buffer free'd on exit without
    // burdening the caller?
    static struct line_t line;
    char read_buf[BUFSZ + 1] = {0};

    if (!line.buf) {
        line.buf = malloc_z(1, LINEBUF_BASE_SZ + 1);
        line.sz = LINEBUF_BASE_SZ;
        line.buf[line.sz] = '\0';
    }

    if (line.len) {
        // Handle edge case of more than one newline
        // having been pulled into the buffer on last read
        int nl;
        if ((nl = find_newline_(line.buf, line.len)) > -1) {
            line.buf[nl] = '\0';
            line.len = nl;
            goto OUT;
        }
    }

    int bytes_read;

    while ((bytes_read = read(fd, read_buf, BUFSZ)) > 0) {
        // Keep buffer clean
        zero(read_buf + bytes_read, BUFSZ - bytes_read);

        // Allocate more space to the line buffer if necessary
        if (line.sz - line.len < bytes_read) {
            // WARN: Only safe if LINEBUF_BASE_SZ >= BUFSZ (see declaration)
            int new_sz = line.sz + LINEBUF_BASE_SZ;

            char *tmp = malloc(new_sz + 1);
            str_cpy(tmp, line.buf);
            free(line.buf);
            line.buf = tmp;

            zero(line.buf + line.sz, new_sz + 1);
            line.sz = new_sz;
        }

        str_cat(line.buf + line.len, read_buf);

        // Find first occurence of newline in buffer
        int nl;
        if ((nl = find_newline_(read_buf, BUFSZ)) > -1) {
            line.len += nl;
            line.buf[line.len] = '\0';
            break;
        } else {
            line.len += bytes_read;
        }
    }

    if (bytes_read < 1)
        return NULL;

OUT:;
    char *out_str = malloc(line.len + 1);
    str_cpy(out_str, line.buf);

    // Shift any data following the newline char to the front of the buffer,
    // ready for next call
    if (line.len < line.sz) {
        // The 1 is so we skip the null terminator
        char *tail = line.buf + line.len + 1;
        int tail_len = str_len(tail);

        mem_move(line.buf, tail);

        // Zero out only what's necessary
        // The remainder of the line buffer should already be zero'd
        int old_len = line.len;
        line.len = tail_len;
        zero(line.buf + line.len + 1, old_len);

    } else {
        // Reset line buffer
        zero(line.buf, line.sz);
        line.len = 0;
    }

    return out_str;
}

int
find_newline_(char *s, int len)
{
    for (int i = 0; i < len; i++) {
        if (s[i] == '\n')
            return i;
    }
    return -1;
}
