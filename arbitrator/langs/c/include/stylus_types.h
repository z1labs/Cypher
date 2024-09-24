// Copyright 2022-2023, Offchain Labs, Inc.
// For licensing, see https://github.com/stylus-sdk-c/blob/stylus/licenses/COPYRIGHT.md

#ifndef __STYLUS_TYPES_H
#define __STYLUS_TYPES_H

/**
 * stylus_types.h defines types used by generated and entrypoint c- macros
 *
 * requires: -
 * c-file: -
 */

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ArbStatus {
    Success = 0,
    Failure,
} ArbStatus;

/**
 * Status = Failure is used to signify a revert
 * Revert or success may both return data to caller
 */
typedef struct ArbResult {
    const ArbStatus status;
    const uint8_t * output;
    const size_t output_len;
} ArbResult;

/**
 * This will create a revert by causing a machine error.
 * There will be no returned data for this event.
 */
inline void revert() {
    asm("unreachable");
}

#ifdef __cplusplus
}
#endif

#endif
