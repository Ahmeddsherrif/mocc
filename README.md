# mocc

`mocc` is a compact C library implementing a dynamic array container with explicit memory management, a small logger, and thread-safe wrapper APIs.

## Quick start

Run the full clean/build/test/benchmark flow:

```bash
bash ./run.sh -l
```

This runs `make clean`, `make build`, `./out/mocc_test`, and `./out/mocc_bench` while capturing stderr logs into `log/`.

## What this repository provides

- `src/mocc.c` and `src/mocc.h`: core dynamic array implementation and public API
- `src/logger.c` and `src/logger.h`: lightweight logging support for assertions and debug output
- `test/mocc_test.c`: unit tests built with the embedded Unity framework
- `bench/mocc_bench.cpp`: benchmark harness for performance evaluation
- `3rd/Unity/`: included Unity test framework sources and documentation
- `run.sh`: convenience script that cleans, builds, runs tests, and runs benchmarks
- `Makefile`: build rules, compiler flags, and targets

## Key features

- dynamic resizing via `mocc_reserve`
- shrink-to-fit support via `mocc_shrink_to_fit`
- append, remove, and random-access operations
- safe error handling using `mocc_error` return codes
- thread-safe wrappers: `mocc_safe_push_back`, `mocc_safe_pop_back`, `mocc_safe_erase`, `mocc_safe_at`, `mocc_safe_front`, `mocc_safe_back`, `mocc_safe_clear`, `mocc_safe_size`, `mocc_safe_capacity`
- C89-compatible source with pthread-based locking
- AddressSanitizer enabled by default for build and test targets

## Repository layout

- `src/`
  - `mocc.c` / `mocc.h`: dynamic array implementation, capacity management, and element operations
  - `logger.c` / `logger.h`: log output helpers used by assertions when `VERBOSE_ENABLED` is active
- `test/`
  - `mocc_test.c`: unit tests that exercise the `mocc` API, including error cases and thread-safe usage
- `bench/`
  - `mocc_bench.cpp`: benchmark executable built with `g++` that links against `libmocc.a`
- `3rd/Unity/`
  - Unity test framework source and support files used by `test/mocc_test.c`
- `out/`
  - created by `make`; contains object files, static library, test executable, and benchmark executable
- `log/`
  - created by `run.sh -l`; stores captured stderr output from test/benchmark runs
- `tmp/`
  - temporary staging directory used by `run.sh` when logging is enabled
- `doc/`
  - additional documentation and repository notes

## Prerequisites

Install the required tools on Ubuntu/Debian:

```bash
sudo apt update
sudo apt install git make gcc g++ clang-format
```

## Clone the repository

Clone the project and initialize the Unity submodule in one step:

```bash
git clone --recurse-submodules https://github.com/Ahmeddsherrif/mocc
```

If the repository was cloned without submodules, run:

```bash
git submodule update --init --recursive
```

## Build and test

Build the library, the unit test executable, and the benchmark executable:

```bash
make build
```

Run the unit tests:

```bash
make test
```

Run the benchmark directly:

```bash
make bench
```

## Using `run.sh`

The script `run.sh` is a convenience wrapper that does the following:

1. runs `make clean`
2. runs `make build`
3. executes `./out/mocc_test`
4. executes `./out/mocc_bench`

To run it with logging enabled, use:

```bash
bash ./run.sh -l
```

When `-l` is provided, `run.sh` captures stderr from the test and benchmark runs. If any stderr output is produced, it is written to a log file in `log/` named like `mocc_log_YYYYMMDD_HHMMSS.log`.

If the file is executable, the same command can be used as:

```bash
./run.sh -l
```

## Notes

- `run.sh -l` does not change the behavior of the build or the executables; it only enables stderr logging during the run.
- The Makefile builds with `-fsanitize=address`, `-DASSERT_ENABLED`, and includes the Unity headers from `3rd/Unity/src`.
- The benchmark binary is built from `bench/mocc_bench.cpp` and linked against `out/libmocc.a`.
