;; This file was generated by https://github.com/wasmerio/wasi-tests

(wasi_test "fd_append.wasm"
  (temp_dirs ".")
  (assert_return (i64.const 0))
  (assert_stdout "\"Hello, world!\\nGoodbye, world!\\n\"\n")
)
