// Copyright 2022-2023, Offchain Labs, Inc.
// For licensing, see https://github.com/stylus-sdk-c/blob/stylus/licenses/COPYRIGHT.md

#ifndef __STYLUS_ENTRY_H
#define __STYLUS_ENTRY_H

/**
 * This defines the entrypoint to a smart contract.
 * Only one file per wasm is expected to have an entrypoint
 *
 * requires: stylus_types.h
 * c-file: -
 */

#include <stddef.h>
#include <stdint.h>

#include "hostio.h"
#include "stylus_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ENTRYPOINT(user_main)                                           \
    /* Force the compiler to import these symbols                    */ \
    /* Note: calling these functions will unproductively consume gas */ \
    __attribute__((export_name("mark_used")))                           \
    void mark_used() {                                                  \
        pay_for_memory_grow(0);                                         \
    }                                                                   \
                                                                        \
    __attribute__((export_name("user_entrypoint")))                     \
    int user_entrypoint(size_t args_len) {                              \
        uint8_t args[args_len];                                         \
        read_args(args);                                                \
        const ArbResult result = user_main(args, args_len);             \
        write_result(result.output, result.output_len);                 \
        return result.status;                                           \
    }

#ifdef __cplusplus
}
#endif

#endif // __STYLUS_ENTRY_H
