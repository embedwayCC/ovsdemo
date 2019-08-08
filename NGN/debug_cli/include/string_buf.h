#ifndef STRING_BUF_H
#define STRING_BUF_H

typedef struct {
    char *buf;
    int len;
    int buf_size;
} string_buf;

int string_buf_init(string_buf *b, int s);
int string_buf_puts(string_buf *b, char *s, int l);
int string_buf_puts_format(string_buf *b, const char *fmt, ...);
int string_buf_free(string_buf *b);

#endif
