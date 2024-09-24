#include <string.h>
#include <bebi.h>

extern inline int bebi_add(bebi lhs, size_t lhs_size, const bebi rhs, size_t rhs_size);
extern inline int bebi_sub(bebi lhs, size_t lhs_size, const bebi rhs, size_t rhs_size);
extern inline int bebi_cmp(const bebi lhs, size_t lhs_size, const bebi rhs, size_t rhs_size);

extern inline void bebi_set_u8(bebi dst, size_t offset, uint8_t val);
extern inline bool bebi_is_zero(const bebi bebi, size_t size);
extern inline uint8_t bebi_get_u8(const bebi src, size_t offset);
extern inline void bebi_set_u16(bebi dst, size_t offset, uint16_t val);
extern inline uint16_t bebi_get_u16(const bebi src, size_t offset);
extern inline void bebi_set_u32(bebi dst, size_t offset, uint32_t val);
extern inline uint32_t bebi_get_u32(const bebi src, size_t offset);
extern inline void bebi_set_u64(bebi dst, size_t offset, uint64_t val);
extern inline uint64_t bebi_get_u64(const bebi src, size_t offset);

int bebi32_add(bebi32 lhs, const bebi32 rhs) {
    return bebi_add(lhs, 32, rhs, 32);
}

int bebi32_sub(bebi32 lhs, const bebi32 rhs) {
    return bebi_sub(lhs, 32, rhs, 32);
}

int bebi32_add_u64(bebi32 lhs, uint64_t rhs) {
    uint8_t rhs_bebi[8];
    bebi_set_u64(rhs_bebi, 0, rhs);
    return bebi_add(lhs, 32, rhs_bebi, 8);
}

void bebi32_set_u8(bebi32 dst, uint8_t val) {
    memset(dst, 0, 32-1);
    bebi_set_u8(dst, 32-1, val);
}

void bebi32_set_u16(bebi32 dst, uint16_t val) {
    memset(dst, 0, 32-2);
    bebi_set_u16(dst, 32-2, val);
}

void bebi32_set_u32(bebi32 dst, uint32_t val) {
    memset(dst, 0, 32-4);
    bebi_set_u32(dst, 32-4, val);
}

void bebi32_set_u64(bebi32 dst, uint64_t val) {
    memset(dst, 0, 32-8);
    bebi_set_u64(dst, 32-8, val);
}

bool bebi32_is_u8(const bebi32 dst) {
    return bebi_is_zero(dst, 32-8);
}

bool bebi32_is_u16(const bebi32 dst) {
    return bebi_is_zero(dst, 32-2);
}

bool bebi32_is_u32(const bebi32 dst) {
    return bebi_is_zero(dst, 32-4);
}

bool bebi32_is_u64(const bebi32 dst) {
    return bebi_is_zero(dst, 32-8);
}

bool bebi32_is_u160(const bebi32 dst) {
    return bebi_is_zero(dst, 32-20);
}

uint16_t bebi32_get_u16(const bebi32 dst) {
    return bebi_get_u16(dst, 32-2);
}

uint32_t bebi32_get_u32(const bebi32 dst) {
    return bebi_get_u32(dst, 32-4);
}

uint64_t bebi32_get_u64(const bebi32 dst) {
    return bebi_get_u64(dst, 32-8);
}

bool bebi32_is_zero(const bebi bebi) {
    return bebi_is_zero(bebi, 32);
}

int bebi32_cmp(const bebi32 lhs, const bebi32 rhs) {
    return bebi_cmp(lhs, 32, rhs, 32);
}
