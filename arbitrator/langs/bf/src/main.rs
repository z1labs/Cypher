// Copyright 2022-2023, Offchain Labs, Inc.
// For licensing, see https://github.com/OffchainLabs/stylus-sdk-bf/blob/stylus/licenses/COPYRIGHT.md

use eyre::Result;
use std::{
    fs::{self, File},
    io::Write,
    path::PathBuf,
};
use structopt::StructOpt;

#[derive(StructOpt)]
#[structopt(name = "bf2wasm")]
struct Opts {
    input: PathBuf,
    #[structopt(short = "o", long)]
    output: Option<PathBuf>,
}

fn main() -> Result<()> {
    let opts = Opts::from_args();
    let Ok(program) = fs::read_to_string(&opts.input) else {
        panic!("failed to read bf file {}", opts.input.to_string_lossy());
    };

    let default_wasm_file = opts.input.with_extension("wasm");
    let wasm_file = match &opts.output {
        Some(file) => file,
        None => &default_wasm_file,
    };
    let wat_path = wasm_file.with_extension("wat");
    let Ok(wat_file) = &mut File::create(&wat_path) else {
        panic!("failed to open wat file {}", wat_path.to_string_lossy());
    };

    write!(wat_file, include_str!("prelude.wat"))?;
    let mut scopes = 1;

    macro_rules! out {
        ($format:expr) => {{
            write!(wat_file, "{: >depth$}", " ", depth = (1 + scopes) * 4)?;
            writeln!(wat_file, $format)?;
        }};
    }

    for c in program.chars() {
        match c {
            '>' => out!("call $right"),
            '<' => out!("call $left"),
            '-' => out!("call $minus"),
            '+' => out!("call $plus"),
            '.' => out!("call $dot"),
            ',' => out!("call $comma"),
            '[' => {
                out!("call $repeat");
                out!("(if (then (loop");
                scopes += 1;
            }
            ']' => {
                out!("call $repeat");
                out!("br_if 0");
                scopes -= 1;
                out!(")))");
            }
            _ => continue,
        }
    }
    scopes = 0;
    out!("))");

    let wasm = fs::read(wat_path)?;
    let wasm = wasmer::wat2wasm(&wasm)?;
    fs::write(wasm_file, wasm)?;
    Ok(())
}
