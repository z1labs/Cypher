#ifndef __STYLUS_UTILS_H
#define __STYLUS_UTILS_H

/**
 * stylus_utils.h defines a few high-level useful utils for stylus smart contracts
 * 
 * requires: bebi.h(string.h), hostio, stylus_types
 * c-file: utils.c
 */

#include <stddef.h>
#include <stdint.h>
#include <stylus_types.h>
#include <hostio.h>
#include <bebi.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * sets message sender inside a padded 32-byte array
 */
inline void msg_sender_padded(bebi sender) {
    msg_sender(sender+12);
}

/**
 * Arbresult with no data, can be success or failure
 */
ArbResult inline _return_nodata(ArbStatus status) {
    ArbResult res = {status, NULL , 0};
    return res;
}

/**
 * string must be short (up to 32 bytes)
 * 
 * Success returns the string in a tuple, as returned by functions returning string
 * Failure returns an Error string in a tuple, which can encode a revert reason for any function
 */
ArbResult _return_short_string(ArbStatus status, char *string);

#ifdef __cplusplus
}
#endif

#endif // __STYLUS_UTILS_H
