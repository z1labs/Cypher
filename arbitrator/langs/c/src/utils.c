#include <stylus_utils.h>
#include <bebi.h>
#include <string.h>

extern inline void msg_sender_padded(bebi sender);

extern ArbResult inline _return_nodata(ArbStatus status);

ArbResult _return_short_string(ArbStatus status, char *string) {
    static uint8_t buf_out[100];
    size_t len = strlen(string);
    if (len > 32) {
        len = 32;
    }
    uint8_t* strlen_out = buf_out+36;
    char *str_out = (char *)(buf_out + 68);
    bebi32_set_u64(strlen_out, len);
    strncpy(str_out, string, 32);
    // Tuple encoding: offset, strlen, str
    bebi32_set_u64(buf_out + 4, 32);
    if (status == Failure) {
        // Err encoding: ErrSignature
        bebi_set_u32(buf_out, 0, 0x08c379a0);
        ArbResult res = {Failure, buf_out, 100};
        return res;
    }
    ArbResult res = {status, buf_out+4, 96};
    return res;
}
