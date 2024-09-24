
#ifndef __STORAGE_H
#define __STORAGE_H

#include <stddef.h>
#include <stdint.h>
#include <bebi.h>
#include <hostio.h>

/**
 * Storage.h creates tools that help access storage from a c-sdk contract
 * 
 * These user is still required to understand solidity storage and use accordingly
 * See: https://docs.soliditylang.org/en/v0.8.20/internals/layout_in_storage.html
 *
 * requires: bebi.h (string.h)
 * c-file: storage.c
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * storage_load / store load or store a value from storage accordingly.
 * 
 * The value of the first "storage" pointer is not used.
 * generated headers provide:
 *  a const storage pointer when working for a view-only function
 *  a non const pointer for a mutating function
 *  no pointer (so don't call storage_load) for a pure function
 *
 */
inline void storage_load(const void* storage, const uint8_t *key, uint8_t *dest) {
    storage_load_bytes32(key, dest);
}

/**
 * see documentation for storage_load
 */
inline void storage_store(void *storage, const uint8_t *key, const uint8_t *value) {
    storage_store_bytes32(key, value);
}

/**
 * calculate slot for a map with base slot "storage" to put "key"
 * If key requires padding it must be applied before calling this function
 */
void map_slot(bebi32 const storage, uint8_t const *key, size_t key_len, bebi32 slot_out);

/**
 * calculate slot and offset for an array with base slot "slot"
 * notice tht short byte-arrays and strings are not stored in base but in
 * the "size" slot - see solidity spec
 */
int array_slot_offset(bebi32 const base, size_t val_size, uint64_t index, bebi32 slot_out, size_t *offset_out);

/**
 * calculate the base slot for a dynamic array with storage-slot "storage"
 * (this is just native keccak of storage into base_out)
 */
void dynamic_array_base_slot(bebi32 const storage, bebi32 base_out);

#ifdef __cplusplus
}
#endif

#endif