
#ifndef __BEBI_H
#define __BEBI_H

/**
 * BEBI stands for Big Endian Big Int
 *
 * It is meant to be used in a small-endian environment, specifically WASM-32
 *
 * It is a library mostly meant to support encoding/decoding of big-endian big-int values
 * Only addition/subtraction and comparisons are supported for math (which is enough for quite a lot)
 * Currently only supporting unsigned.
 *  
 * c-file: bebi.c
 * requires: string.h
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * bebi type does not have a predefined size or alignment requirements
 */
typedef uint8_t bebi[];

/**
 * Set/get functions allow extracting or setting a u8/16/32/64 from any offset inside the bebi
 */
inline void bebi_set_u8(bebi dst, size_t offset, uint8_t val);
inline void bebi_set_u16(bebi dst, size_t offset, uint16_t val);
inline void bebi_set_u32(bebi dst, size_t offset, uint32_t val);
inline void bebi_set_u64(bebi dst, size_t offset, uint64_t val);

inline uint8_t bebi_get_u8(const bebi src, size_t offset);
inline uint16_t bebi_get_u16(const bebi src, size_t offset);
inline uint32_t bebi_get_u32(const bebi src, size_t offset);
inline uint64_t bebi_get_u64(const bebi src, size_t offset);

/**
 * Adds rhs into lhs. Lhs must be at least as long as rhs.
 *  
 * return values:
 * -1 : ERROR rhs_size > lhs_size
 * 0 : O.k
 * 1 : O.k but there was an overflow
 */
inline int bebi_add(bebi lhs, size_t lhs_size, const bebi rhs, size_t rhs_size);

/**
 * Subtracts rhs from lhs. Lhs must be at least as long as rhs.
 *  
 * return values:
 * -1 : ERROR rhs_size > lhs_size
 * 0 : O.k
 * 1 : O.k but there was an overflow
 */
inline int bebi_sub(bebi lhs, size_t lhs_size, const bebi rhs, size_t rhs_size);

/**
 * compares two values
 *  
 * return values:
 * lhs > rhs: 1
 * lhs < rhs: -1
 * lhs == rhs: 0
 */
inline int bebi_cmp(const bebi lhs, size_t lhs_size, const bebi rhs, size_t rhs_size);

inline bool bebi_is_zero(const bebi bebi, size_t size);

/**
 * bebi32 is a specialized bebi of size 32, which is used a lot in solidity
 * there are no alignment requirements.
 */
typedef uint8_t bebi32[32];

/**
 * bebi32 get/set functions are different from generic bebi.
 * according to solidity standards - they require the value to be stored in
 * the last (least significant) bytes, padded by zeroes
 *
 * setting a bebi32 to e.g. uint32 will pad it with zeroes accordingly
 *  
 * get functions don't test if value is padded - testing can be done separately
 * by the bebi32_is_*
 *
 * notice you can use generic bebi variants for a bebi32 when they fit better
 */
uint16_t bebi32_get_u16(const bebi32 dst);
uint32_t bebi32_get_u32(const bebi32 dst);
uint64_t bebi32_get_u64(const bebi32 dst);

void bebi32_set_u8(bebi32 dst, uint8_t val);
void bebi32_set_u16(bebi32 dst, uint16_t val);
void bebi32_set_u32(bebi32 dst, uint32_t val);
void bebi32_set_u64(bebi32 dst, uint64_t val);

bool bebi32_is_u8(const bebi32 dst);
bool bebi32_is_u16(const bebi32 dst);
bool bebi32_is_u32(const bebi32 dst);
bool bebi32_is_u64(const bebi32 dst);
bool bebi32_is_u160(const bebi32 dst);

int bebi32_add(bebi32 lhs, const bebi32 rhs);
int bebi32_sub(bebi32 lhs, const bebi32 rhs);
int bebi32_add_u64(bebi32 lhs, uint64_t rhs);

int bebi32_cmp(const bebi32 lhs, const bebi32 rhs);
bool bebi32_is_zero(const bebi bebi);


/******* implementation of previously-declated functions *********/
inline int bebi_add(bebi lhs, size_t lhs_size, const bebi rhs, size_t rhs_size) {
    if (rhs_size > lhs_size) {
        return -1;
    }
    size_t left = lhs_size;
    size_t right = rhs_size;
    uint8_t carry = 0;
    while (left > 0 && right > 0) {
        left--;
        right--;
        uint8_t res = lhs[left] + rhs[right] + carry;
        carry = res < lhs[left] ? 1 : 0;
        lhs[left] = res;
    }
    while (left > 0 && carry) {
        left--;
        uint8_t res = lhs[left] + carry;
        carry = res < lhs[left] ? 1 : 0;
        lhs[left] = res;
    }
    return carry;
}

inline int bebi_sub(bebi lhs, size_t lhs_size, const bebi rhs, size_t rhs_size) {
    if (rhs_size > lhs_size) {
        return -1;
    }
    size_t left = lhs_size;
    size_t right = rhs_size;
    uint8_t carry = 0;
    while (left > 0 && right > 0) {
        left--;
        right--;
        uint8_t res = lhs[left] - rhs[right] - carry;
        carry = res > lhs[left] ? 1 : 0;
        lhs[left] = res;
    }
    while (left > 0 && carry) {
        left--;
        uint8_t res = lhs[left] - carry;
        carry = res > lhs[left] ? 1 : 0;
        lhs[left] = res;
    }
    return carry;
}

inline int bebi_cmp(const bebi lhs, size_t lhs_size, const bebi rhs, size_t rhs_size) {
    size_t left = 0;
    size_t right = 0;
    while (lhs_size - left > rhs_size) {
        if (lhs[left] != 0) {
            return 1;
        }
        left++;
    }
    while (rhs_size - right > lhs_size) {
        if (rhs[right] != 0) {
            return -1;
        }
        right++;
    }
    while (left < lhs_size) {
        if (lhs[left] > rhs[right]) {
            return 1;
        }
        if (lhs[left] < rhs[right]) {
            return -1;
        }
        right++;
        left++;
    }
    return 0;
}

inline void bebi_set_u8(bebi dst, size_t offset, uint8_t val) {
    dst[offset] = val;
}

inline bool bebi_is_zero(const bebi bebi, size_t size) {
    size_t idx = 0;
    while (idx < size) {
        if (bebi[idx] != 0) {
            return false;
        }
        idx++;
    }
    return true;
}

inline uint8_t bebi_get_u8(const bebi src, size_t offset) {
    return src[offset];
}

inline void bebi_set_u16(bebi dst, size_t offset, uint16_t val) {
    dst[offset+1] = val & 0xff;
    val >>= 8;
    dst[offset] = val & 0xff;
}

inline uint16_t bebi_get_u16(const bebi src, size_t offset) {
    uint16_t val;
    val = src[offset];
    val <<= 8;
    val |= src[offset+1];
    return val;
}

inline void bebi_set_u32(bebi dst, size_t offset, uint32_t val) {
    dst[offset+3] = val & 0xff;
    val >>= 8;
    dst[offset+2] = val & 0xff;
    val >>= 8;
    dst[offset+1] = val & 0xff;
    val >>= 8;
    dst[offset] = val & 0xff;
}

inline uint32_t bebi_get_u32(const bebi src, size_t offset) {
    uint32_t val;
    val = src[offset];
    val <<= 8;
    val |= src[offset+1];
    val <<= 8;
    val |= src[offset+2];
    val <<= 8;
    val |= src[offset+3];
    return val;
}

inline void bebi_set_u64(bebi dst, size_t offset, uint64_t val) {
    dst[offset+7] = val & 0xff;
    val >>= 8;
    dst[offset+6] = val & 0xff;
    val >>= 8;
    dst[offset+5] = val & 0xff;
    val >>= 8;
    dst[offset+4] = val & 0xff;
    val >>= 8;
    dst[offset+3] = val & 0xff;
    val >>= 8;
    dst[offset+2] = val & 0xff;
    val >>= 8;
    dst[offset+1] = val & 0xff;
    val >>= 8;
    dst[offset] = val & 0xff;
}

inline uint64_t bebi_get_u64(const bebi src, size_t offset) {
    uint64_t val;
    val = src[offset];
    val <<= 8;
    val |= src[offset+1];
    val <<= 8;
    val |= src[offset+2];
    val <<= 8;
    val |= src[offset+3];
    val <<= 8;
    val |= src[offset+4];
    val <<= 8;
    val |= src[offset+5];
    val <<= 8;
    val |= src[offset+6];
    val <<= 8;
    val |= src[offset+7];
    return val;
}

#ifdef __cplusplus
}
#endif

#endif // __BEBI_H