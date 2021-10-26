/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef READ_LINE_H
#define READ_LINE_H

extern int str_len(const char *s);
extern char *str_cpy(char *dest, const char *src);
extern char *str_cat(char *dest, const char *src);
extern char * mem_move(char *dest, const char *src);
extern void zero(char *src, int nbytes);
extern char * malloc_z(int sz, int nmemb);

struct line_t {
    char *p;    // Line buffer
    int sz;     // Allocated size in bytes
    int len;    // Length of string stored at `p`, not including '\0'
};

char *read_line(int fd);

#endif /* READ_LINE_H */
