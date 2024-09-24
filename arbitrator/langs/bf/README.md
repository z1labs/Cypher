<br />
<p align="center">
  <a href="https://arbitrum.io/">
    <img src="https://arbitrum.io/assets/stylus/stylus_with_paint_bg.png" alt="Logo" width="100%">
  </a>

  <h3 align="center">The Stylus SDK</h3>

  <p align="center">
    <a href="https://docs.arbitrum.io/stylus/stylus-gentle-introduction"><strong>Bf contracts on Arbitrum »</strong></a>
    <br />
  </p>
</p>

## Overview

[Bf][wiki] is a humorous, esoteric programming language lauded for its minimalism, difficulty, and ability to produce inscrutable code for even the simplest of programs. For example, here's the shortest known implementation of [Hello World][hello], by KSab.

```brainfuck
+[>>>->-[>->----<<<]>>]>.---.>+..+++.>>.<.>>---.<<<.+++.------.<-.>>+.
```

The 8 symbols seen in the above constitute the entirety of the Bf programming language. Because their operations simulate a [Turing Machine][Turing], Bf can be used to write any computable function. That is, anything you can do in C, Rust, etc, you can do in Bf &mdash; if you can figure out how to write it!

For better or worse, this repo includes a Bf-to-WebAssembly compiler, which allows Bf programs to run at near-native speeds on Arbitrum chains. The symbols are interpreted as follows.

| Op  | Effect                                                          |
|:---:|:----------------------------------------------------------------|
| `>` | Move the machine's head 1 cell to the right.                    |
| `<` | Move the machine's head 1 cell to the left.                     |
| `+` | Increment the byte stored in the current cell.                  |
| `-` | Decrement the byte stored in the current cell.                  |
| `.` | Append the current cell's byte to the EVM return data.          |
| `,` | Read the next byte of calldata into the current cell.           |
| `[` | Jump to the matching `]` if the current cell's byte is `0`.     |
| `]` | Jump to the matching `[` if the current cell's byte is not `0`. |

[wiki]: https://esolangs.org/wiki/Brainfuck
[hello]: https://tio.run/##HYpBCoAwEAMftGxeEPKR0oMWCiJ4EHz/mnYOQ0hyvsf1zG/cVdEkpbItGZJd6oIzFEBEQAKtVXnfVW5An/yq@gE
[Turing]: https://en.wikipedia.org/wiki/Turing_machine

## Usage

To invoke the compiler, run
```sh
cargo run <input.b> -o <output.wat>
```

To upload the `.wat` to a Stylus-enabled Arbitrum chain, see [`cargo stylus`][cargo].

[cargo]: https://github.com/OffchainLabs/stylus-sdk-bf

## Why does this exist?

Though seemingly just for fun, we hope this repo will be of educational value to framework developers. Creating Stylus SDKs for new languages is surprisingly straightforward, and uses the same building blocks seen in the generated `.wat` files this Bf compiler produces. One can even deploy hand-written `.wat` files using the imports seen in [`prelude.wat`][prelude];

```wat
(module
    (import "vm_hooks" "read_args"    (func $read_args   (param i32    )))
    (import "vm_hooks" "write_result" (func $return_data (param i32 i32)))

    (func $main (export "user_entrypoint") (param $args_len i32) (result i32)
        ;; your code here
    )
)
```

All it takes is a WebAssembly-enabled compiler and a few imports, the full list of which can be found [here][hostios]. The table below includes our official SDKs built on the same ideas.

| Repo             | Use cases                   | License           |
|:-----------------|:----------------------------|:------------------|
| [Rust SDK][Rust] | Everything!                 | Apache 2.0 or MIT |
| [C/C++ SDK][C]   | Cryptography and algorithms | Apache 2.0 or MIT |
| [Bf SDK][Bf]     | Educational                 | Apache 2.0 or MIT |

Want to write your own? Join us in the `#stylus` channel on [discord][discord]!

[prelude]: https://github.com/OffchainLabs/stylus-sdk-bf/blob/stylus/src/prelude.wat
[hostios]: https://github.com/OffchainLabs/stylus-sdk-rs/blob/stylus/stylus-sdk/src/hostio.rs

[Rust]: https://github.com/OffchainLabs/stylus-sdk-rs
[C]: https://github.com/OffchainLabs/stylus-sdk-c
[Bf]: https://github.com/OffchainLabs/stylus-sdk-bf

[discord]: https://discord.com/invite/5KE54JwyTs

## License

&copy; 2022-2023 Offchain Labs, Inc.

This project is licensed under either of

- [Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0) ([licenses/Apache-2.0](licenses/Apache-2.0))
- [MIT license](https://opensource.org/licenses/MIT) ([licenses/MIT](licenses/MIT))

at your option.

The [SPDX](https://spdx.dev) license identifier for this project is `MIT OR Apache-2.0`.
