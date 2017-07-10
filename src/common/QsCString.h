#ifndef QS_CSTRING_H
#define QS_CSTRING_H

#include "aos_define.h"

AOS_CPP_START

typedef struct {
    int len;
    char *data;
} qs_string_t;

#define qs_string(str)     { sizeof(str) - 1, (char *) str }
#define qs_null_string     { 0, NULL }
#define qs_str_set(str, text)                                  \
    (str)->len = strlen(text); (str)->data = (char *) text
#define qs_str_null(str)   (str)->len = 0; (str)->data = NULL

#define aos_tolower(c)      (char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define aos_toupper(c)      (char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

static APR_INLINE void qs_string_tolower(aos_string_t *str)
{
    int i = 0;
    while (i < str->len) {
        str->data[i] = aos_tolower(str->data[i]);
        ++i;
    }
}

static APR_INLINE char *aos_strlchr(char *p, char *last, char c)
{
    while (p < last) {
        if (*p == c) {
            return p;
        }
        p++;
    }
    return NULL;
}

/*
static APR_INLINE int aos_is_quote(char c)
{
    return c == '\"';
}

static APR_INLINE int aos_is_space(char c)
{
    return ((c == ' ') || (c == '\t'));
}

static APR_INLINE int aos_is_space_or_cntrl(char c)
{
    return c <= ' ';
}

*/

static int qs_is_null_string(aos_string_t *str)
{
    if (str == NULL || str->data == NULL || str->len == 0) {
        return AOS_TRUE;
    }
    return AOS_FALSE;


#endif
