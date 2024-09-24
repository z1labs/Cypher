;; Copyright 2022-2023, Offchain Labs, Inc.
;; For licensing, see https://github.com/OffchainLabs/stylus-sdk-bf/blob/stylus/licenses/COPYRIGHT.md

(module
    (import "vm_hooks" "read_args"    (func $read_args   (param i32    )))
    (import "vm_hooks" "write_result" (func $return_data (param i32 i32)))
    (memory (export "memory") 1 1)

    ;; args advances byte by byte
    (global $args_ptr (mut i32) (i32.const 0x00))
    (global $args_end (mut i32) (i32.const 0x00))

    ;; outs just extends the length
    (global $outs_ptr i32 (i32.const 0x400))
    (global $outs_len (mut i32) (i32.const 0))
    (global $outs_cap i32 (i32.const 0x400))

    (global $cell (mut i32) (i32.const 0x800))

    ;; sets up the entry point
    (func $main (export "user_entrypoint") (param $args_len i32) (result i32)

        ;; load the args
        local.get $args_len
        global.set $args_end
        global.get $args_ptr
        call $read_args

        ;; set up the pointer
        global.get $outs_ptr
        global.get $outs_cap
        i32.add
        global.set $cell

        ;; call the generated program
        call $user

        ;; write the outs
        global.get $outs_ptr
        global.get $outs_len
        call $return_data

        ;; return status success
        i32.const 0)

    ;; reads one byte from the args
    (func $comma
        ;; read the value
        global.get $args_ptr
        global.get $args_end
        i32.eq
        (if (then
                ;; at the end, write a 0
                global.get $cell
                i32.const 0
                i32.store8
                return))

        ;; write the value
        global.get $cell
        global.get $args_ptr
        i32.load8_u
        i32.store8

        ;; advance 1 byte
        global.get $args_ptr
        i32.const 1
        i32.add
        global.set $args_ptr)

    ;; writes one byte to the outs
    (func $dot
        ;; noop when out of space
        global.get $outs_len
        global.get $outs_cap
        i32.eq
        (if (then (return)))

        ;; store the value
        global.get $outs_ptr
        global.get $outs_len
        i32.add
        global.get $cell
        i32.load8_u
        i32.store8

        ;; advance by 1
        global.get $outs_len
        i32.const 1
        i32.add
        global.set $outs_len)

    (func $right
        global.get $cell
        i32.const 1
        i32.add
        global.set $cell)

    (func $left
        global.get $cell
        i32.const 1
        i32.sub
        global.set $cell)

    (func $plus
        global.get $cell
        global.get $cell
        i32.load8_u
        i32.const 1
        i32.add
        i32.store8)

    (func $minus
        global.get $cell
        global.get $cell
        i32.load8_u
        i32.const 1
        i32.sub
        i32.store8)

    (func $repeat (result i32)
        global.get $cell
        i32.load8_u
        i32.const 0
        i32.ne)

    ;; user program
    (func $user
