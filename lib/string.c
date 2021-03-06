#include <string.h>

void memcpy(void *dst, const void *src, unsigned int num)
{
	const unsigned char *s = src;
	unsigned char *d = dst;

	while (num--) {
		*d++ = *s++;
	}
}

void *memset(void *s, int c, unsigned int count)
{
	char *xs = (char *)s;

	while (count--)
		*xs++ = c;

	return s;
}

int strcmp(const char *cs, const char *ct)
{
	char res;

	while (1) {
		if ((res = *cs - *ct++) != 0 || !*cs++)
			break;
	}

	return res;
}

void *memchr(const void *ptr, int value, size_t num)
{
    const unsigned char *p = ptr;

    while (num--) {
        if (*p == value) {
            return (void *) p;
        }
        p++;
    }

    return NULL;
}

int memcmp(const void *ptr1, const void *ptr2, size_t num)
{
    const unsigned char *p1 = ptr1;
    const unsigned char *p2 = ptr2;

    while (num--) {
        if (*p1 != *p2) {
            if (*p1 > *p2) {
                return 1;
            }
            else {
                return -1;
            }
        }

        p1++;
        p2++;
    }

    return 0;
}

void memmove(void *dst, const void *src, size_t n)
{
    const char *s = src;
    char *d = dst;
    if ((unsigned int *)s < (unsigned int *)d)
        while(n--) d[n] = s[n];
    else
        while(n--) *d++ = *s++;
}

size_t strlen(const char *s)
{
    size_t len = 0;
    while (*s++) {
        len++;
    }
    return len;
}

char *strcpy(char *dst, const char *src)
{
	while (*src)
		*dst++ = *src++;

	return dst;
}
