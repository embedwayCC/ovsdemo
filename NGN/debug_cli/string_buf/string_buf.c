#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "string_buf.h"

//
// -- stringbuffer.c
//
// 动态字符串,根据追加的内容自动分配memory
// 提供puts和puts_format两种追加方法
//

int string_buf_init(string_buf *b, int s)
{
    b->buf=malloc(s);
    b->buf_size = s;
    b->len=0;
    return 0;
}

int string_buf_puts(string_buf *b, char *s, int l)
{
    while( b->len + l > b->buf_size ) {
		#if 0
        b->buf = realloc(b->buf,b->buf_size*2);
        b->buf_size = b->buf_size*2;
		#else
        b->buf = realloc(b->buf,b->len + l + 1);
        b->buf_size = b->len + l;
		#endif
    }
    strncpy(b->buf+b->len,s,l);
    b->len  = b->len + l ;
    b->buf[b->len]=0;
    return 0;
}


int string_buf_puts_format(string_buf *b, const char *fmt, ...)
{
    int n;
    int size = 100;     /* Guess we need no more than 100 bytes. */
    char *p, *np;
    va_list ap;

    if ((p = malloc(size)) == NULL)
        return -1;

    while (1) {

        /* Try to print in the allocated space. */

        va_start(ap, fmt);
        n = vsnprintf(p, size, fmt, ap);
        va_end(ap);

        /* If that worked, return the string. */

        if (n > -1 && n < size)
        {
            string_buf_puts(b, p, n);
            free(p);
            return 0;
        }

        /* Else try again with more space. */

        if (n > -1)    /* glibc 2.1 */
            size = n+1; /* precisely what is needed */
        else           /* glibc 2.0 */
            size *= 2;  /* twice the old size */

        if ((np = realloc (p, size)) == NULL) {
            free(p);
            return -1;
        } else {
            p = np;
        }
    }
}

int string_buf_free(string_buf *b)
{
	if(b->buf){
		free(b->buf);
	}
    b->buf= NULL;
    b->buf_size = 0;
    b->len=0;
    return 0;
}

