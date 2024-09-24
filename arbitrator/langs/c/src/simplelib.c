#include <string.h>

extern inline void *memcpy(void *destination, const void *source, size_t num);
extern inline void *memset(void *ptr, int value, size_t num);

char *strncpy(char *dst, const char *src, size_t num) {
    size_t idx=0;
    while (idx<num && src[idx]!=0) {
        idx++;
    }
    memcpy(dst, src, idx);
    if (idx < num) {
        memset(dst+num, 0, num-idx);
    }
    return dst;
}

size_t strlen(const char *str) {
    size_t idx=0;
    while (str[idx]!=0) {
        idx++;
    }
    return idx;
}
