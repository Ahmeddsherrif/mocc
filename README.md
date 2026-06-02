# mocc

`mocc` is a compact dynamic array container implemented in C with a small, testable runtime. It provides vector-style operations while keeping memory management explicit and error-safe.

## What this repository provides

- `src/mocc.c` and `src/mocc.h`: dynamic array implementation and public interface
- `test/mocc_test.c`: extensive Unity-based unit tests covering edge cases, capacity growth, and thread-safe wrappers
- `bench/mocc_bench.c`: benchmark harness for performance evaluation
- `3rd/Unity/`: embedded Unity test framework submodule

## Key features

- dynamic resizing with `mocc_reserve` and `mocc_shrink_to_fit`
- append and remove operations: `mocc_push_back`, `mocc_pop_back`, `mocc_erase`
- random access via `mocc_at`, `mocc_front`, and `mocc_back`
- error reporting through `mocc_error` return codes
- thread-safe wrappers for concurrent use: `mocc_safe_*` APIs
- built with C89/C90-compatible code and pthread-based locking

## Getting started

### Prerequisites

Install the required tools on Ubuntu/Debian:

```bash
sudo apt update
sudo apt install git make gcc g++
```

If you prefer the full development toolchain:

```bash
sudo apt install build-essential git
```

### Clone the repository

Clone the project and initialize the Unity submodule:

```bash
git clone --recurse-submodules https://github.com/Ahmeddsherrif/mocc
```

If the repo is already cloned without submodules:

```bash
git submodule update --init --recursive
```

## Build and test

Build the project, including the library, unit tests, and benchmark executable:

```bash
make build
```

Run the full unit test suite:

```bash
make test
```

Or build and execute the test binary manually:

```bash
make out/mocc_test
./out/mocc_test
```

## Benchmark

Build and run the benchmark executable:

```bash
make bench
```

## Clean generated files

Remove the build output directory:

```bash
make clean
```

## Notes

- The unit tests include concurrency validation for the `mocc_safe_*` thread-safe APIs.
- The build uses AddressSanitizer (`-fsanitize=address`) for safer memory diagnostics.
- The repository follows a simple layout:
  - `src/` — core implementation
  - `test/` — unit test code
  - `bench/` — benchmark code
  - `3rd/Unity/` — Unity framework
  - `doc/` — documentation and task details
