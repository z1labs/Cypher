// SPDX-License-Identifier: MIT

pragma solidity ^0.8.20;

/**
 * This contract exists to implement similar logic to the c-example ERC20.
 * It exists for documentation and testing of the c-example.
 *
 * This contract is:
 *  * NOT Audited / well tested
 *  * NOT a good solidity example.
 *  * NOT a good or a safe way to to manage minters for an ERC20.
 *
 * The implementation here is specifically meant to be easily comparable to
 * the C implementation.
 * The C implementation itself is meant only as an example code.
 *
 */

contract Equivalent {
    // total supply of the token
    uint256 private _totalSupply;

    // how many minters exist currently
    uint64 public minters_current;
    // has the contract been initialized
    bool  private initialized;

    // array of minters
    // The array could hold blank values when removing a minter
    // slot 0 is always empty
    address[] public minters;

    // balances of each account
    mapping(address account => uint256) private _balances;

    // a map from minters to their index in minters array
    // also used to check if an address is a minter
    mapping(address minter => uint64) public minter_idx;


    // Standard pure functions of ERC20
    function name() public pure returns (string memory) {
        return "C Token (sample code)";
    }

    // Standard pure functions of ERC20
    function symbol() public pure returns (string memory) {
        return "CTOK";
    }

    // Standard pure functions of ERC20
    function decimals() public pure returns (uint8) {
        return 18;
    }

    // solidity accessor to the private field
    function totalSupply() public view returns (uint256) {
        return _totalSupply;
    }

    // solidity accessor to the private field
    function balanceOf(address account) public view returns (uint256) {
        return _balances[account];
    }

    // standard ERC20: move "value" tokens from message sender to "to"
    function transfer(address to, uint256 value) public returns (bool) {
        uint256 current_source = _balances[msg.sender];
        // check if sender has enough balance
        if (current_source < value) {
            return false;
        }
        uint256 current_dest = _balances[to];
        require(current_dest + value > current_dest, "overflow");
        _balances[msg.sender] = current_source - value;
        _balances[to] = current_dest + value;
        return true;
    }

    /**
     * Minters
     * 
     * A call to init initializes the contract with one minter
     * Each minter may mint tokens, add or remove other minters
     *
     * Minters are kept in two databases: an array of all minters
     * (may contain blanks), and a map from minter to it's index
     * in the array. Index 0 is always blank so index 0 in the map
     * is used to signify an address that's not a minter.
     */

    // push_minter adds to the end of minters_array, and updates minter_idx map
    function push_minter(address minter) internal {
        uint64 index = uint64(minters.length);
        minters.push(minter);
        // address 0 is never marked "minter"
        if (uint160(minter) != 0) {
            minter_idx[minter] = index;
        }
    }

    // set the first minter
    function init(address first_minter) public {
        // revert with reason string
        require(!initialized, "already initialized");
        // first blank entry reserves index 0 for non-minters
        push_minter(address(uint160(0)));
        // push the first minter
        push_minter(first_minter);
        // set minters_current and initialized
        initialized = true;
        minters_current = 1;
    }

    // helper function: check if message was sent from a minter
    function from_minter() internal view returns (bool) {
        return minter_idx[msg.sender] != 0;
    }

    // add a new minter
    function add_minter(address new_minter) public {
        require(from_minter(), "must be aminter");
        minters_current+=1;
        push_minter(new_minter);
    }

    // remove minter
    // array entry is replaced with blank
    // minter_idx map entry is removed
    function remove_minter(address old_minter) public {
        require(from_minter(), "must be aminter");
        uint64 old_idx = minter_idx[old_minter];
        require(old_idx!=0, "remove: not minter");
        minter_idx[old_minter];
        minters_current-=1;
        minters[old_idx] = address(uint160(0));
    }

    // mint new tokens for an account
    function mint(address account, uint256 value) public {
        require(from_minter(), "must be aminter");
        require(_totalSupply + value >= _totalSupply);
        _totalSupply += value;
        _balances[account] += value;
    }

    // TODO: complete the ERC20 interface
    // function allowance(address owner, address spender) public view virtual returns (uint256);
    // function approve(address spender, uint256 value) public virtual returns (bool);
    // function transferFrom(address from, address to, uint256 value) public virtual returns (bool);
    // function increaseAllowance(address spender, uint256 addedValue) public virtual returns (bool);
    // function decreaseAllowance(address spender, uint256 requestedDecrease) public virtual returns (bool);
}