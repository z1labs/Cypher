#include <string.h>
#include <stylus_types.h>
#include <bebi.h>
#include <erc20/ERC20.h>
#include <storage.h>
#include <stdbool.h>
#include <stylus_debug.h>
#include <stylus_utils.h>

/**
 * Implementation of the C ERC20-style contract.
 *
 * Also see:
 *  * erc20.sol - declaring the interface that's generated
 *  * Makefile - the process of building the different parts
 *  * equiv.sol - equivalent solidity implementation
 *
 * This contract is:
 *  * NOT Audited / well tested
 *  * NOT a good or a safe way to to manage minters for an ERC20.
 * 
 * It is an example code explaining how to do things in stylus C-SDK.
 * 
 * Notes to the reader:
 * 1) Different languages are good at different thing.
 * Unsurprisingly, solidity is very good at solidity-compatibility.
 * C requires low level understanding and implementation of things
 * that are ready out-of-the box in solidity.
 * On the other hand, C with stylus allows you to do a level of process
 * power that is just not available to solidity smart contracts.
 *
 * 2) A C-SDK smart contract should conform to solidity for it's public
 * ABI to allow easy interaction with other contracts.
 * Storage compatibility is a matter of choice and standard.
 * The strongest argument for compatibility is it allows external tools
 * to find data easily, and it allows contract upgrades across languages.
 *
 */

/**
 * General utils/helpers
 */

// buffer used to write output, avoiding malloc
uint8_t buf_out[32];

// succeed and return a bebi32
ArbResult inline _success_bebi32(bebi32 const retval) {
    ArbResult res = {Success, retval , 32};
    return res;
}

/**
 * Storage access helpers
 */

// initialized and minters_current are in the same slot.
// We access both these "short" values in the same function to remain in control of
// the number of underline SLOAD/SSTORE operations
void inline load_shorts(const void *storage, uint64_t *minters_current_out, bool *initialized_out) {
    bebi32 storage_slot = STORAGE_SLOT_minters_current;
    bebi32 buf;
    storage_load(storage, storage_slot, buf);
    if (minters_current_out != NULL) {
        // END_OFFSET tells us where the value ends, we load from where it starts.
        uint64_t res = bebi_get_u64(buf, STORAGE_END_OFFSET_minters_current - sizeof(uint64_t));
        *minters_current_out = res;
    }
    if (initialized_out != NULL) {
        uint8_t res = bebi_get_u8(buf, STORAGE_END_OFFSET_initialized - sizeof(uint8_t));
        *initialized_out = (res != 0);
    }
}

// always initialized==1 when storing
void inline store_shorts(void *storage, uint64_t minters_current) {
    bebi32 storage_slot = STORAGE_SLOT_minters_current;
    bebi32 buf;
    bebi_set_u64(buf, STORAGE_END_OFFSET_minters_current - sizeof(uint64_t), minters_current);
    bebi_set_u8(buf, STORAGE_END_OFFSET_initialized - sizeof(uint8_t), 1);
    storage_store(storage, storage_slot, buf);
}

// calculate slot for minter_idx map of a minter
void minter_idx_storage_slot(bebi32 const minter, bebi32 slot_out) {
    bebi32 base = STORAGE_SLOT_minter_idx;
    map_slot(base, minter, 32, slot_out);
}

// calculate slot for balances map of an account
void balance_slot(bebi32 const account, bebi32 slot_out) {
    bebi32 base = STORAGE_SLOT__balances;
    map_slot(base, account, 32, slot_out);    
}

// get index of a minter from minter_idx map
uint64_t _minter_idx(const void *storage, bebi32 const minter) {
    bebi32 slot;
    minter_idx_storage_slot(minter, slot);
    bebi32 buffer;
    storage_load(storage, slot, buffer);
    return bebi32_get_u64(buffer);
}


/**
 * Implementing the contract's API
 * 
 * These functions implement API defined by the solidity interface,
 * and/or have comparable implementation in equiv.sol
 */

// the default function is called if input doesn't match any selector
ArbResult default_func(void *storage, uint8_t *input, size_t len, bebi32 value) {
    // This will cause a revert with a reason strong, which can help debug
    return _return_short_string(Failure, "not supported");
}

// Standard pure functions of ERC20
ArbResult name(uint8_t *input, size_t len){ // name()
    if (len != 0) {
        return _return_nodata(Failure);
    }
    return _return_short_string(Success, "C Token (sample code)");
}

// Standard pure functions of ERC20
ArbResult symbol(uint8_t *input, size_t len) { // symbol()
    if (len != 0) {
        return _return_nodata(Failure);
    }
    return _return_short_string(Success, "CTOK");
}

// Standard pure functions of ERC20// Standard pure functions of ERC20
ArbResult decimals(uint8_t *input, size_t len) { // decimals()
    if (len != 0) {
        return _return_nodata(Failure);
    }
    bebi32_set_u8(buf_out, 18);
    return _success_bebi32(buf_out);
}

// accessor to the _totalSupply field
ArbResult totalSupply(const void *storage, uint8_t *input, size_t len){ // totalSupply()
    if (len != 0) {
        return _return_nodata(Failure);
    }
    bebi32 total_supply_slot = STORAGE_SLOT__totalSupply;

    storage_load(storage, total_supply_slot, buf_out);
    return _success_bebi32(buf_out);
}

// standard ERC20: a read accessor to the balances map
ArbResult balanceOf(const void *storage, uint8_t *input, size_t len) { // balanceOf(address)
    // validate input is an address padded to 32 bytes
    if (len != 32) {
        return _return_nodata(Failure);
    }
    if (!bebi32_is_u160(input)) {
        return _return_nodata(Failure);
    }
    // calculate slot for address
    bebi32 slot;
    balance_slot(input, slot);
    // load and return value
    storage_load(storage, slot, buf_out);
    return _success_bebi32(buf_out);    
}

// standard ERC20:  move "value" tokens from message sender to "to"
ArbResult transfer(void *storage, uint8_t *input, size_t len) { // transfer(address,uint256)
    // input should be two bytes32: destination (an address) and amount (uint256)
    if (len != 64) {
        return _return_nodata(Failure);
    }
    // const pointers to the two values
    uint8_t const *dest = input;
    uint8_t const *amount = (input + 32);
    // test if destination is an address
    if (!bebi32_is_u160(dest)) {
        return _return_nodata(Failure);
    }

    // read message sender
    bebi32 sender;
    msg_sender_padded(sender);
    // calculate the storage slot for sender's balance
    bebi32 balance_slot_buf;
    balance_slot(sender, balance_slot_buf);
    // read the sender's balance
    bebi32 balance_buf;
    storage_load(storage, balance_slot_buf, balance_buf);
    // check if sender has enough balance
    if (bebi32_cmp(balance_buf, amount) < 0) {
        // return false
        // notice we return a Success here because we do not revert
        bebi32_set_u8(buf_out, 0);
        return _success_bebi32(buf_out);
    }
    // reduce sender balance and store into the same slot
    bebi32_sub(balance_buf, amount);
    storage_store(storage, balance_slot_buf, balance_buf);

    // load/increase/store receiver balance
    balance_slot(dest, balance_slot_buf);
    storage_load(storage, balance_slot_buf, balance_buf);
    int overflow = bebi32_add(balance_buf, amount);
    if (overflow) {
        return _return_nodata(Failure);
    }
    storage_store(storage, balance_slot_buf, balance_buf);

    // return true
    bebi32_set_u8(buf_out, 1);
    return _success_bebi32(buf_out);
}

// push_minter adds to the end of minters_array, and updates minter_idx map
void push_minter(void *storage, bebi32 minter) {
    // read array size
    bebi32 array_size_slot = STORAGE_SLOT_minters;
    bebi32 array_size;
    storage_load(storage, array_size_slot, array_size);
    // in solidity array size is uint256 - but a going over 1<<64
    // is not feasible, so for C we use a u64 index.
    if (!bebi32_is_u64(array_size)) {
        revert();
    }
    uint64_t index = bebi32_get_u64(array_size);

    // address 0 is never marked "minter"
    if (!bebi32_is_zero(minter)) {
        // compute memory slot for the index
        bebi32 idx_slot;
        minter_idx_storage_slot(minter, idx_slot);
        // store the u64 index into a 32-byte value
        bebi32 idx_bebi;
        bebi32_set_u64(idx_bebi, index);
        // store value
        storage_store(storage, idx_slot, idx_bebi);
    }

    // calculate storage slot in the end of the array
    bebi32 minters_slot;
    bebi32 minters_base = STORAGE_BASE_minters;
    array_slot_offset(minters_base, 32, index, minters_slot, NULL);
    // store minter into it's slot
    storage_store(storage, minters_slot, minter);
    // store the increased array size into it's slot
    bebi32_set_u64(array_size, index+1);
    storage_store(storage, array_size_slot, array_size);
}

// set the first minter
ArbResult init(void *storage, uint8_t *input, size_t len) { // init()
    // validate_input
    if (len != 32 || !bebi32_is_u160(input)) {
        return _return_nodata(Failure);
    }
    bool initialized;
    load_shorts(storage, NULL, &initialized);
    if (initialized) {
        // revert with reason string
        return _return_short_string(Failure, "already initialized");
    }
    // first blank entry reserves index 0 for non-minters
    bebi32 zero_minter = {0};
    push_minter(storage, zero_minter);
    // push the first minter
    push_minter(storage, input);
    // store_shorts sets minters_current and initialized
    store_shorts(storage, 1);
    return _return_nodata(Success);
}

// helper function: check if message was sent from a minter
bool from_minter(const void *storage) {
    bebi32 sender;
    msg_sender_padded(sender);
    return  _minter_idx(storage, sender) != 0;
}

// add a new minter
ArbResult add_minter(void *storage, uint8_t *input, size_t len) { // add_minter(address)
    if (!from_minter(storage)) {
        //revert with reason string
        return _return_short_string(Failure, "must be a minter");
    }
    if (len != 32 || !bebi32_is_u160(input)) {
        return _return_nodata(Failure);
    }
    uint64_t minters_current;
    load_shorts(storage, &minters_current, NULL);

    push_minter(storage, input);
    minters_current +=1;
    store_shorts(storage, minters_current);
    return _return_nodata(Success);
}

// remove minter
// array entry is replaced with blank
// minter_idx map entry is removed
ArbResult remove_minter(void *storage, uint8_t *input, size_t len) { // remove_minter(address)
    if (!from_minter(storage)) {
        //revert with reason string
        return _return_short_string(Failure, "must be a minter");
    }
    if (len != 32 || !bebi32_is_u160(input)) {
        return _return_nodata(Failure);
    }
    // compute slot and load minter index
    bebi32 idx_slot;
    bebi32 buf;
    minter_idx_storage_slot(input, idx_slot);
    storage_load(storage, idx_slot, buf);
    uint64_t idx = bebi32_get_u32(buf);
    if (!idx) {
        //revert with reason string
        return  _return_short_string(Failure, "remove: not minter");
    }
    bebi32 zero;
    memset(zero, 0, 32);

    // remove index from map
    storage_store(storage, idx_slot, zero);

    // compute slot of minter in the minters array
    bebi32 minters_slot;
    bebi32 minters_base = STORAGE_SLOT_minters;
    array_slot_offset(minters_base, 32, idx, minters_slot, NULL);
    // store 0 in the minters array
    storage_store(storage, minters_slot, zero);

    // reduce minters_current count
    uint64_t minters_current;
    load_shorts(storage, &minters_current, NULL);
    store_shorts(storage, minters_current-1);
    return _return_nodata(Success);
}

// mint new tokens for an account
ArbResult mint(void *storage, uint8_t *input, size_t len){ // mint(address,uint256)
    if (!from_minter(storage)) {
        //revert with reason string
        return _return_short_string(Failure, "must be a minter");
    }
    if (len != 64) {
        return _return_nodata(Failure);
    }
    uint8_t const *dest = input;
    uint8_t const *amount = (input + 32);
    // check dest is an address
    if (!bebi32_is_u160(dest)) {
        return _return_nodata(Failure);
    }
    // compute slot and load balance
    bebi32 balance_slot_buf;
    balance_slot(dest, balance_slot_buf);
    bebi32 balance_buf;
    storage_load(storage, balance_slot_buf, balance_buf);
    // update balance (make sure there is no overflow)
    int overflow = bebi32_add(balance_buf, amount);
    if (overflow) {
        return _return_nodata(Failure);
    }
    storage_store(storage, balance_slot_buf, balance_buf);

    //update total-supply (make sure there is no overflow)
    bebi32 total_supply_slot = STORAGE_SLOT__totalSupply;
    bebi32 total_supply;
    storage_load(storage, total_supply_slot, total_supply);
    overflow = bebi32_add(total_supply, amount);
    if (overflow) {
        return _return_nodata(Failure);
    }
    storage_store(storage, total_supply_slot, total_supply);
    return _return_nodata(Success);
}

/**
 * Public field accessors
 * 
 * Below functions are implicitly created in solidity - because these are public
 * fields in the contract.
 * 
 * No free lunches in C - we need to implement these accessors as well.
 */

ArbResult minters(const void *storage, uint8_t *input, size_t len) { // minters(uint256)
    // test input is a valid uint256
    if (len != 32) {
        return _return_nodata(Failure);
    }
    // get array size
    bebi32 array_size_slot = STORAGE_SLOT_minters;
    bebi32 array_size_buf;
    storage_load(storage, array_size_slot, array_size_buf);
    uint64_t array_size = bebi32_get_u64(array_size_buf);

    // check index is within bounds
    uint64_t index = bebi32_get_u64(input);
    if (!bebi32_is_u64(input) || index >= array_size) {
        // revert with reason
        return _return_short_string(Failure, "index out of bounds");
    }
    // load entry and return it
    bebi32 minters_slot;
    bebi32 minters_base = STORAGE_SLOT_minters;
    array_slot_offset(minters_base, 32, index, minters_slot, NULL);
    storage_load(storage, minters_slot, buf_out);
    return _success_bebi32(buf_out);
}

ArbResult minters_current(const void *storage, uint8_t *input, size_t len) { // minters_current()
    if (len != 0) {
        return _return_nodata(Failure);
    }
    uint64_t minters_current;
    load_shorts(storage, &minters_current, NULL);
    bebi32_set_u64(buf_out, minters_current);
    return _success_bebi32(buf_out);
}

ArbResult minter_idx(const void *storage, uint8_t *input, size_t len) { // minter_idx(address)
    if (len != 32 || !bebi32_is_u160(input)) {
        return _return_nodata(Failure);
    }
    uint64_t idx = _minter_idx(storage, input);
    bebi32_set_u64(buf_out, idx);
    return _success_bebi32(buf_out);
}

