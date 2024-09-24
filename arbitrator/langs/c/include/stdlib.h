#ifndef __SIMPLELIB_STDLIB_H
#define __SIMPLELIB_STDLIB_H

/**
 * Not at all a full implementation of stdlib.h,
 * just the parts of it supported by simplelib
 * 
 * requirements: -
 * c-file: stdlib.c
 */

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void *malloc(size_t size);
void free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif // __SIMPLELIB_STDLIB_H
