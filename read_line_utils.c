#include <stdlib.h>
#ifndef NULL
#define NULL 0
#endif

/*
 * String length
 */
int
str_len(const char *s)
{
    int len = 0;
    while (s[len++] != '\0');
    return --len;
}

/*
 * Copy string
 * Assumes `dest` is large enough.
 */
char *
str_cpy(char *dest, const char *src)
{
    char *ret = dest;
    while (*src != '\0')
        *dest++ = *src++;
    *dest = '\0';
    return ret;
}

/*
 * Append string
 * Both strings are expected to be null terminated.
 * Assumes that `dest` is large enough to store the additional content
 * from `src`.
 */
char *
str_cat(char *dest, const char *src)
{
    char *cur = dest;
    while (*cur != '\0')
        cur++;
    while (*src != '\0')
        *cur++ = *src++;
    *cur = '\0';
    return dest;
}

/*
 * Copy memory
 * `src` and `dest` may overlapa.
 * Assumes `dest` is at least as large as `src`.
 */
char *
mem_move(char *dest, const char *src)
{
    int buf_len = str_len(src);
    char *buf = malloc(buf_len + 1);
    str_cpy(buf, src);
    str_cpy(dest, buf);
    free(buf);
    return dest;
}

void zero(char *src, int nbytes)
{
    if (!src)
        return;

    for (int i = 0; i < nbytes; i++) {
        src[i] = '\0';
    }
}

/*
 * Allocate memory and set to all zeroes.
 */
char *
malloc_z(int sz, int nmemb)
{
    int bytes = nmemb * sz;
    char *mem = malloc(bytes);
    zero(mem, bytes);
    return mem;
}
