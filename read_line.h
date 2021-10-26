/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef READ_LINE_H
#define READ_LINE_H

char *read_line(int fd);

extern int str_len(const char *s);
extern char *str_cpy(char *dest, const char *src);
extern char *str_cat(char *dest, const char *src);
extern char *mem_move(char *dest, const char *src);
extern void zero(char *src, int nbytes);
extern char *malloc_z(int sz, int nmemb);

#endif /* READ_LINE_H */
