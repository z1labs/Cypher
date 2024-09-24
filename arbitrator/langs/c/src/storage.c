#include <hostio.h>
#include <string.h>
#include <bebi.h>

int array_slot_offset(bebi32 const base, size_t val_size, uint64_t index, bebi32 slot_out, size_t *offset_out) {
    uint64_t slots;
    uint64_t offset;
    if (val_size == 0) {
        return -1;
    }
    if (val_size >= 32) {
        offset = 0;
        slots = index * ((val_size + 31)/ 32);
    } else {
        uint64_t per_slot = (32 / val_size);
        slots = index / per_slot;
        offset = index % per_slot;
    }
    memcpy(slot_out, base, 32);
    bebi32_add_u64(slot_out, slots);
    if (offset_out != NULL) {
        *offset_out = offset;
    }
    return 0;
}

void dynamic_array_base_slot(bebi32 const storage, bebi32 base_out) {
    native_keccak256(storage, 32, base_out);
}

void map_slot(bebi32 const storage, uint8_t const *key, size_t key_len, bebi32 slot_out) {
    uint8_t buf[32 + key_len];
    memcpy(buf, key, key_len);
    memcpy(buf+key_len, storage, 32);
    native_keccak256(buf, 32 + key_len, slot_out);
}

