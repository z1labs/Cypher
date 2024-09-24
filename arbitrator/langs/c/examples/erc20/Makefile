
STACK_SIZE=8192
CC=clang
LD=wasm-ld
CFLAGS=-I../../include/ -Iinterface-gen/ --target=wasm32 -Os --no-standard-libraries -mbulk-memory -Wall -g
LDFLAGS=-O2 --no-entry --stack-first -z stack-size=$(STACK_SIZE) -Bstatic

OBJECTS=build/impl.o build/lib/bebi.o build/lib/storage.o build/lib/simplelib.o build/lib/utils.o build/gen/ERC20_main.o

all: ./erc20.wasm

# STEP1 : compile solidity
# The output is used by cargo and can also be used by any web3 for ABI
# For interface_compile.json see: https://docs.soliditylang.org/en/v0.8.20/using-the-compiler.html#compiler-input-and-output-json-description
build/interface.json: interface_compile.json erc20.sol
	mkdir -p build
	cat $< | solc --standard-json --pretty-json > $@

# STEP 2 : generate headers and main function from the compiled solidity
cargo-generate: build/interface.json
	mkdir -p build
	cargo-stylus c-gen $< interface-gen

interface-gen/erc20/ERC20_main.c: cargo-generate

# Step 3.1: build the generated main file (ERC20_main.o)
build/gen/%.o: interface-gen/erc20/%.c
	mkdir -p build/gen/
	$(CC) $(CFLAGS) -c $< -o $@

# Step 3.2: build the reuqired library files
build/lib/%.o: ../../src/%.c
	mkdir -p build/lib
	$(CC) $(CFLAGS) -c $< -o $@

# STEP 3.3: implement / build the functions creating the logic of the smart contract
build/%.o: %.c cargo-generate
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

# Step 4: link
build/erc20.wasm: $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

# Step 5: strip symbols (they won't help on-chain)
erc20.wasm: build/erc20.wasm
	wasm-strip -o $@ $<

# Step 6: check the wasm using cargo-stylus
# cargo stylus check --wasm-file-path ./erc20.wasm  --endpoint $ENDPOINT --private-key=$PRIVATE_KEY

# Step 7: deploy the wasm using cargo-stylus
# cargo stylus check --wasm-file-path ./erc20.wasm  --endpoint $ENDPOINT --private-key=$PRIVATE_KEY

clean:
	rm -rf interface-gen build erc20.wasm

.phony: all cargo-generate clean
