// Copyright 2022-2023, Offchain Labs, Inc.
// For licensing, see https://github.com/stylus-sdk-c/blob/stylus/licenses/COPYRIGHT.md

#ifndef STYLUS_DEBUG_H
#define STYLUS_DEBUG_H

/**
 * These functions are only usable in a debug-enabled network, which will usually be local-dev environments
 * Prints will appear on the dev-node's log.
 */

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CONSOLE(name) extern __attribute__((import_module("console"),  import_name(#name)))

/**
 * Prints a 32-bit floating point number to the console, Only available in debug mode with
 * floating point enabled.
 */
CONSOLE(log_f32) void log_f32(float * value);

/**
 * Prints a 64-bit floating point number to the console, Only available in debug mode with
 * floating point enabled.
 */
CONSOLE(log_f64) void log_f64(double * value);

/**
 * Prints a 32-bit integer to the console, which can be either signed or unsigned.
 * Only available in debug mode.
 */
CONSOLE(log_i32) void log_i32(int32_t value);

/**
 * Prints a 64-bit integer to the console, which can be either signed or unsigned.
 * Only available in debug mode.
 */
CONSOLE(log_i64) void log_i64(int64_t value);

/**
 * Prints a UTF-8 encoded string to the console. Only available in debug mode.
 */
CONSOLE(log_txt) void log_txt(const uint8_t * text, size_t len);

#ifdef __cplusplus
}
#endif

#endif
