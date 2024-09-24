// Copyright 2022-2023, Offchain Labs, Inc.
// For licensing, see https://github.com/stylus-sdk-c/blob/stylus/licenses/COPYRIGHT.md

#ifndef STYLUS_HOSTIO_H
#define STYLUS_HOSTIO_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VM_HOOK(name) extern __attribute__((import_module("vm_hooks"), import_name(#name)))

/**
 * Gets the ETH balance in wei of the account at the given address.
 * The semantics are equivalent to that of the EVM’s [`BALANCE`] opcode.
 * 
 * [`BALANCE`]: https://www.evm.codes/#31
 */
VM_HOOK(account_balance) void account_balance(const uint8_t * address, uint8_t * dest);

/**
 * Gets a subset of the code from the account at the given address. The semantics are identical to that
 * of the EVM's [`EXT_CODE_COPY`] opcode, aside from one small detail: the write to the buffer `dest` will
 * stop after the last byte is written. This is unlike the EVM, which right pads with zeros in this scenario.
 * The return value is the number of bytes written, which allows the caller to detect if this has occured.
 * 
 * [`EXT_CODE_COPY`]: https://www.evm.codes/#3C
 */
VM_HOOK(account_code) size_t account_code(const uint8_t * address, size_t offset, size_t size, uint8_t * code);

/**
 * Gets the size of the code in bytes at the given address. The semantics are equivalent
 * to that of the EVM's [`EXT_CODESIZE`].
 * 
 * [`EXT_CODESIZE`]: https://www.evm.codes/#3B
 */
VM_HOOK(account_code_size) size_t account_code_size(const uint8_t * address);

/**
 * Gets the code hash of the account at the given address. The semantics are equivalent
 * to that of the EVM's [`EXT_CODEHASH`] opcode. Note that the code hash of an account without
 * code will be the empty hash
 * `keccak("") = c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470`.
 * 
 * [`EXT_CODEHASH`]: https://www.evm.codes/#3F
 */
VM_HOOK(account_codehash) void account_codehash(const uint8_t * address, uint8_t * dest);

/**
 * Reads a 32-byte value from permanent storage. Stylus's storage format is identical to
 * that of the EVM. This means that, under the hood, this hostio is accessing the 32-byte
 * value stored in the EVM state trie at offset `key`, which will be `0` when not previously
 * set. The semantics, then, are equivalent to that of the EVM's [`SLOAD`] opcode.
 * 
 * [`SLOAD`]: https://www.evm.codes/#54
 */
VM_HOOK(storage_load_bytes32) void storage_load_bytes32(const uint8_t * key, uint8_t * dest);

/**
 * Stores a 32-byte value to permanent storage. Stylus's storage format is identical to that
 * of the EVM. This means that, under the hood, this hostio is storing a 32-byte value into
 * the EVM state trie at offset `key`. Furthermore, refunds are tabulated exactly as in the
 * EVM. The semantics, then, are equivalent to that of the EVM's [`SSTORE`] opcode.
 * 
 * [`SSTORE`]: https://www.evm.codes/#55
 */
VM_HOOK(storage_cache_bytes32) void storage_cache_bytes32(const uint8_t * key, const uint8_t * value);

/**
 * Gets the basefee of the current block. The semantics are equivalent to that of the EVM's
 * [`BASEFEE`] opcode.
 * 
 * [`BASEFEE`]: https://www.evm.codes/#48
 */
VM_HOOK(block_basefee) void block_basefee(uint8_t * basefee);

/**
 * Gets the unique chain identifier of the Arbitrum chain. The semantics are equivalent to
 * that of the EVM's [`CHAIN_ID`] opcode.
 * 
 * [`CHAIN_ID`]: https://www.evm.codes/#46
 */
VM_HOOK(chainid) uint64_t chainid();

/**
 * Gets the coinbase of the current block, which on Arbitrum chains is the L1 batch poster's
 * address. This differs from Ethereum where the validator including the transaction
 * determines the coinbase.
 */
VM_HOOK(block_coinbase) void block_coinbase(uint8_t * coinbase);

/**
 * Gets the gas limit of the current block. The semantics are equivalent to that of the EVM's
 * [`GAS_LIMIT`] opcode. Note that as of the time of this writing, `evm.codes` incorrectly
 * implies that the opcode returns the gas limit of the current transaction.  When in doubt,
 * consult [`The Ethereum Yellow Paper`].
 * 
 * [`GAS_LIMIT`]: https://www.evm.codes/#45
 * [`The Ethereum Yellow Paper`]: https://ethereum.github.io/yellowpaper/paper.pdf
 */
VM_HOOK(block_gas_limit) uint64_t block_gas_limit();

/**
 * Gets a bounded estimate of the L1 block number at which the Sequencer sequenced the
 * transaction. See [`Block Numbers and Time`] for more information on how this value is
 * determined.
 * 
 * [`Block Numbers and Time`]: https://developer.arbitrum.io/time
 */
VM_HOOK(block_number) uint64_t block_number();

/**
 * Gets a bounded estimate of the Unix timestamp at which the Sequencer sequenced the
 * transaction. See [`Block Numbers and Time`] for more information on how this value is
 * determined.
 * 
 * [`Block Numbers and Time`]: https://developer.arbitrum.io/time
 */
VM_HOOK(block_timestamp) uint64_t block_timestamp();

/**
 * Calls the contract at the given address with options for passing value and to limit the
 * amount of gas supplied. The return status indicates whether the call succeeded, and is
 * nonzero on failure.
 * 
 * In both cases `return_data_len` will store the length of the result, the bytes of which can
 * be read via the `read_return_data` hostio. The bytes are not returned directly so that the
 * programmer can potentially save gas by choosing which subset of the return result they'd
 * like to copy.
 * 
 * The semantics are equivalent to that of the EVM's [`CALL`] opcode, including callvalue
 * stipends and the 63/64 gas rule. This means that supplying the `u64::MAX` gas can be used
 * to send as much as possible.
 * 
 * [`CALL`]: https://www.evm.codes/#f1
 */
VM_HOOK(call_contract) uint8_t call_contract(
        const uint8_t * contract,
        const uint8_t * calldata,
        const size_t calldata_len,
        const uint8_t * value,
        const uint64_t gas,
        size_t * return_data_len
);

/**
 * Gets the address of the current program. The semantics are equivalent to that of the EVM's
 * [`ADDRESS`] opcode.
 * 
 * [`ADDRESS`]: https://www.evm.codes/#30
 */
VM_HOOK(contract_address) void contract_address(uint8_t * address);

/**
 * Deploys a new contract using the init code provided, which the EVM executes to construct
 * the code of the newly deployed contract. The init code must be written in EVM bytecode, but
 * the code it deploys can be that of a Stylus program. The code returned will be treated as
 * WASM if it begins with the EOF-inspired header `0xEFF000`. Otherwise the code will be
 * interpreted as that of a traditional EVM-style contract. See [`Deploying Stylus Programs`]
 * for more information on writing init code.
 * 
 * On success, this hostio returns the address of the newly created account whose address is
 * a function of the sender and nonce. On failure the address will be `0`, `return_data_len`
 * will store the length of the revert data, the bytes of which can be read via the
 * `read_return_data` hostio. The semantics are equivalent to that of the EVM's [`CREATE`]
 * opcode, which notably includes the exact address returned.
 * 
 * [`Deploying Stylus Programs`]: https://developer.arbitrum.io/TODO
 * [`CREATE`]: https://www.evm.codes/#f0
 */
VM_HOOK(create1) void create1(
        const uint8_t * code,
        const size_t code_len,
        const uint8_t * endowment,
        uint8_t * contract,
        size_t * revert_data_len
);

/**
 * Deploys a new contract using the init code provided, which the EVM executes to construct
 * the code of the newly deployed contract. The init code must be written in EVM bytecode, but
 * the code it deploys can be that of a Stylus program. The code returned will be treated as
 * WASM if it begins with the EOF-inspired header `0xEFF000`. Otherwise the code will be
 * interpreted as that of a traditional EVM-style contract. See [`Deploying Stylus Programs`]
 * for more information on writing init code.
 * 
 * On success, this hostio returns the address of the newly created account whose address is a
 * function of the sender, salt, and init code. On failure the address will be `0`,
 * `return_data_len` will store the length of the revert data, the bytes of which can be read
 * via the `read_return_data` hostio. The semantics are equivalent to that of the EVM's
 * `[CREATE2`] opcode, which notably includes the exact address returned.
 * 
 * [`Deploying Stylus Programs`]: https://developer.arbitrum.io/TODO
 * [`CREATE2`]: https://www.evm.codes/#f5
 */
VM_HOOK(create2) void create2(
        const uint8_t * code,
        const size_t code_len,
        const uint8_t * endowment,
        const uint8_t * salt,
        uint8_t * contract,
        size_t * revert_data_len
);

/**
 * Delegate calls the contract at the given address, with the option to limit the amount of
 * gas supplied. The return status indicates whether the call succeeded, and is nonzero on
 * failure.
 * 
 * In both cases `return_data_len` will store the length of the result, the bytes of which
 * can be read via the `read_return_data` hostio. The bytes are not returned directly so that
 * the programmer can potentially save gas by choosing which subset of the return result
 * they'd like to copy.
 * 
 * The semantics are equivalent to that of the EVM's [`DELEGATE_CALL`] opcode, including the
 * 63/64 gas rule. This means that supplying `u64::MAX` gas can be used to send as much as
 * possible.
 * 
 * [`DELEGATE_CALL`]: https://www.evm.codes/#F4
 */
VM_HOOK(delegate_call_contract) uint8_t delegate_call_contract(
        const uint8_t * contract,
        const uint8_t * calldata,
        const size_t calldata_len,
        const uint64_t gas,
        size_t * return_data_len
);

/**
 * Emits an EVM log with the given number of topics and data, the first bytes of which should
 * be the 32-byte-aligned topic data. The semantics are equivalent to that of the EVM's
 * [`LOG0`], [`LOG1`], [`LOG2`], [`LOG3`], and [`LOG4`] opcodes based on the number of topics
 * specified. Requesting more than `4` topics will induce a revert.
 * 
 * [`LOG0`]: https://www.evm.codes/#a0
 * [`LOG1`]: https://www.evm.codes/#a1
 * [`LOG2`]: https://www.evm.codes/#a2
 * [`LOG3`]: https://www.evm.codes/#a3
 * [`LOG4`]: https://www.evm.codes/#a4
 */
VM_HOOK(emit_log) void emit_log(uint8_t * data, size_t len, size_t topics);

/**
 * Gets the amount of gas left after paying for the cost of this hostio. The semantics are
 * equivalent to that of the EVM's [`GAS`] opcode.
 * 
 *  [`GAS`]: https://www.evm.codes/#5a
 */
VM_HOOK(evm_gas_left) uint64_t evm_gas_left();

/**
 * Gets the amount of ink remaining after paying for the cost of this hostio. The semantics
 * are equivalent to that of the EVM's [`GAS`] opcode, except the units are in ink. See
 * [`Ink and Gas`] for more information on Stylus's compute pricing.
 * 
 * [`GAS`]: https://www.evm.codes/#5a
 * [`Ink and Gas`]: https://developer.arbitrum.io/TODO
 */
VM_HOOK(evm_ink_left) uint64_t evm_ink_left();

/**
 * The `ENTRYPOINT` macro handles importing this hostio, which is required if the
 * program's memory grows. Otherwise compilation through the `ArbWasm` precompile will revert.
 * Internally the Stylus VM forces calls to this hostio whenever new WASM pages are allocated.
 * Calls made voluntarily will unproductively consume gas.
 */
VM_HOOK(pay_for_memory_grow) void pay_for_memory_grow(const uint16_t pages);

/** 
 * Gets the address of the account that called the program. For normal L2-to-L2 transactions
 * the semantics are equivalent to that of the EVM's [`CALLER`] opcode, including in cases
 * arising from [`DELEGATE_CALL`].
 * 
 * For L1-to-L2 retryable ticket transactions, the top-level sender's address will be aliased.
 * See [`Retryable Ticket Address Aliasing`] for more information on how this works.
 * 
 * [`CALLER`]: https://www.evm.codes/#33
 * [`DELEGATE_CALL`]: https://www.evm.codes/#f4
 * [`Retryable Ticket Address Aliasing`]: https://developer.arbitrum.io/arbos/l1-to-l2-messaging#address-aliasing
 */
VM_HOOK(msg_sender) void msg_sender(const uint8_t * sender);

/**
 * Get the ETH value in wei sent to the program. The semantics are equivalent to that of the
 * EVM's [`CALLVALUE`] opcode.
 * 
 * [`CALLVALUE`]: https://www.evm.codes/#34
 */
VM_HOOK(msg_value) void msg_value(const uint8_t * value);

/**
 * Efficiently computes the [`keccak256`] hash of the given preimage.
 * The semantics are equivalent to that of the EVM's [`SHA3`] opcode.
 * 
 * [`keccak256`]: https://en.wikipedia.org/wiki/SHA-3
 * [`SHA3`]: https://www.evm.codes/#20
 */
VM_HOOK(native_keccak256) void native_keccak256(const uint8_t * bytes, size_t len, uint8_t * output);

/**
 * Reads the program calldata. The semantics are equivalent to that of the EVM's
 * [`CALLDATA_COPY`] opcode when requesting the entirety of the current call's calldata.
 * 
 * [`CALLDATA_COPY`]: https://www.evm.codes/#37
 */
VM_HOOK(read_args) void read_args(const uint8_t * data);

/**
 * Copies the bytes of the last EVM call or deployment return result. Does not revert if out of
 * bounds, but rather copies the overlapping portion. The semantics are otherwise equivalent
 * to that of the EVM's [`RETURN_DATA_COPY`] opcode.
 * 
 * Returns the number of bytes written.
 * 
 * [`RETURN_DATA_COPY`]: https://www.evm.codes/#3e
 */
VM_HOOK(read_return_data) size_t read_return_data(uint8_t * dest, size_t offset, size_t size);

/**
 * Writes the final return data. If not called before the program exists, the return data will
 * be 0 bytes long. Note that this hostio does not cause the program to exit, which happens
 * naturally when [`user_entrypoint`] returns.
 */
VM_HOOK(write_result) void write_result(const uint8_t * data, size_t len);

/**
 * Returns the length of the last EVM call or deployment return result, or `0` if neither have
 * happened during the program's execution. The semantics are equivalent to that of the EVM's
 * [`RETURN_DATA_SIZE`] opcode.
 *
 * [`RETURN_DATA_SIZE`]: https://www.evm.codes/#3d
 */
VM_HOOK(return_data_size) size_t return_data_size();

/**
 * Static calls the contract at the given address, with the option to limit the amount of gas
 * supplied. The return status indicates whether the call succeeded, and is nonzero on
 * failure.
 * 
 * In both cases `return_data_len` will store the length of the result, the bytes of which can
 * be read via the `read_return_data` hostio. The bytes are not returned directly so that the
 * programmer can potentially save gas by choosing which subset of the return result they'd
 * like to copy.
 * 
 * The semantics are equivalent to that of the EVM's [`STATIC_CALL`] opcode, including the
 * 63/64 gas rule. This means that supplying `u64::MAX` gas can be used to send as much as
 * possible.
 * 
 * [`STATIC_CALL`]: https://www.evm.codes/#FA
 */
VM_HOOK(static_call_contract) uint8_t static_call_contract(
        const uint8_t * contract,
        const uint8_t * calldata,
        const size_t calldata_len,
        const uint64_t gas,
        size_t * return_data_len
);

/**
 * Gets the gas price in wei per gas, which on Arbitrum chains equals the basefee. The
 *  semantics are equivalent to that of the EVM's [`GAS_PRICE`] opcode.
 * 
 *  [`GAS_PRICE`]: https://www.evm.codes/#3A
 */
VM_HOOK(tx_gas_price) void tx_gas_price(uint8_t * gas_price);

/**
 * Gets the price of ink in evm gas basis points. See [`Ink and Gas`] for more information on
 * Stylus's compute-pricing model.
 * 
 * [`Ink and Gas`]: https://developer.arbitrum.io/TODO
 */
VM_HOOK(tx_ink_price) uint64_t tx_ink_price();

/**
 * Gets the top-level sender of the transaction. The semantics are equivalent to that of the
 * EVM's [`ORIGIN`] opcode.
 * 
 * [`ORIGIN`]: https://www.evm.codes/#32
 */
VM_HOOK(tx_origin) void tx_origin(uint8_t * origin);

#ifdef __cplusplus
}
#endif

#endif
