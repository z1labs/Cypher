// Copyright 2022-2023, Offchain Labs, Inc.
// For licensing, see https://github.com/OffchainLabs/stylus-sdk-c/blob/stylus/licenses/COPYRIGHT.md
//
// You can compile this file with stock clang as follows
//     clang *.c -o siphash.wasm --target=wasm32 --no-standard-libraries -mbulk-memory -Wl,--no-entry -O3
//
// For C programs that use the standard library, cross compile clang with wasi if your compiler doesn't support it
//     https://github.com/WebAssembly/wasi-sdk

#include "stylus_entry.h"

// siphash impl from siphash.c
extern uint64_t siphash24(const void *src, unsigned long len, const uint8_t key[16]);

// the entrypoint
ArbResult user_main(uint8_t * args, size_t args_len) {
    const uint8_t * key = args;
    const uint8_t * input = args + 16;
    const uint64_t length = args_len - 16;

    // call siphash on the args
    uint64_t hash = siphash24(input, length, key);

    // re-use args for efficiency
    __builtin_memcpy(args, &hash, sizeof(hash));

    return (ArbResult) {
        .status = Success,
        .output = args,
        .output_len = sizeof(hash),
    };
}

// sets user_main as the entrypoint
ENTRYPOINT(user_main);
