// SPDX-License-Identifier: MIT

/**
 * Solidity interface - declaration
 *
 * Interface is inspired by ERC20, with minters that can be added/removed by
 * other minters
 *
 * Documentation in this file does not explain the ABI or contract, but how 
 * c-code is generated from the solidity.
 * This file will be compiled by solidity compiler and used to generate:
 *   * ABI that can be used by any web3 client
 *   * C code framework (headers and main function) for the smart contract
 *
 * Also see: Makefile, interface_compile.json
 *
 * This file is based on the MIT-license implementation of ERC20 by OpenZepplin
 * See original implementation: https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC20/IERC20.sol
 * Original License: https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/LICENSE
 */

pragma solidity ^0.8.20;

/**
 * Name of the contract will set name of files generated. Name of the solidity file
 * in interface_compile.json sets the name of the directory.
 * This will create: erc20/ERC20.h and erc20/ERC20_main.c
 *
 * abstract contract can contain functions without implementation. 
 *
 * generated c only cares about storage and function declarations as they appear here.
 */
abstract contract ERC20 {
    /**
     * PART I - storage
     *
     * in the generated .h file, two defines will be created for each storage variable:
     * STORAGE_SLOT__* - 32-bytes initializer with the appropriate base_slot
     * STORAGE_OFFSET__* - offset inside that slot for the variable
     *
     * see also:
     *   *  https://docs.soliditylang.org/en/v0.8.20/internals/layout_in_storage.html
     *   *  storage.h
     */

    /**
     * First variable is in slot zero. This will generate:
     * #define STORAGE_SLOT__totalSupply {0x00, 0x00, ... 0x00, 0x00, 0x00}
     */
    uint256 private _totalSupply;

    /**
     * Both these variables share a slot. A STORAGE_SLOT_* define will be created for each, all are identical.
     * The offsets generated, however, are not:
     *
     * #define STORAGE_END_OFFSET_minters_current 32
     * #define STORAGE_END_OFFSET_initialized 24
     *
     * These are offsets to the END of the values, as the name suggests, so the minters_current u64
     * can be read from bytes [24-32), and the "initialized" boolean from the byte [23-24) 
     */
    uint64 public minters_current;
    bool  initialized;

    /**
     * A dynamic array stores it's size in (slot) and values starting in keccak256(slot)
     * so in addition to the slot value, base is also generated in the header file:
     *
     * #define STORAGE_BASE_minters {0x40, 0x57, 0x87, ... 0xbb, 0x5a, 0xce}
     */
    address[] public minters;

    /**
     * for maps the base is not hashed. see storage.h for utils helping to calculate position of hashed values
     */
    mapping(address account => uint256) private _balances;

    /**
     * notice: defining a variable as public also creates a getter function (here: minter_idx(address) returns uint64)
     * you will have to implement this function as well.
     */
    mapping(address spender => uint64) public minter_idx;


    /**
     * PART II - functions
     *
     * Only need to define public/external functions for the ABI (no difference for our use).
     * Define all functions virtual to avoid requiring a solidity implementation.
     *
     * The generated h file will contain declarations for all functions declared here and selectors
     * for them.
     * 
     * The generated _main file will contain an entry point that parses the function selector and
     * calls the appropriate function.
     *
     * The programmer's should implement the functions declared in the generated h-file.
     *
     * a default payable function will be called if input doesn't match any selector.
     */

    /*
     * Pure functions should not access storage.
     * This is the generated c declaration:
     *
     * ArbResult symbol(uint8_t *input, size_t len);
     *
     * see stylus_types for definition of ArbResult.
     * input and input_length don't include the function selector
     * the function should validate it's input and parse output accordingly
     */
    function name() public pure virtual returns (string memory);
    function symbol() public pure virtual returns (string memory);
    function decimals() public pure virtual returns (uint8);

    /*
     * A view function can read storage.
     * This is the generated c declaration:
     *
     * ArbResult totalSupply(const void *storage, uint8_t *input, size_t len); // totalSupply()
     *
     * the storage pointer isn't really set or used, but should be passed to storage functions
     * for compile-time verification that we should access storage. see storage.h
     */
    function totalSupply() public view virtual returns (uint256);
    function balanceOf(address account) public view virtual returns (uint256);

    /**
     * a mutating functions gets a non-const storage pointer:
     *
     * ArbResult mint(void *storage, uint8_t *input, size_t len); // mint(address,uint256)
     * 
     * Notice again that apart from function-selector, the environment does not parse or
     * encode any input/output variable unless explicitly done by the function implementor
     */
    function mint(address account, uint256 value) public virtual;

    function transfer(address to, uint256 value) public virtual returns (bool);

    function init(address first_minter) public virtual;
    function add_minter(address new_minter) public virtual;
    function remove_minter(address old_minter) public virtual;

    // TODO: complete the ERC20 interface
    // function allowance(address owner, address spender) public view virtual returns (uint256);
    // function approve(address spender, uint256 value) public virtual returns (bool);
    // function transferFrom(address from, address to, uint256 value) public virtual returns (bool);
    // function increaseAllowance(address spender, uint256 addedValue) public virtual returns (bool);
    // function decreaseAllowance(address spender, uint256 requestedDecrease) public virtual returns (bool);
}