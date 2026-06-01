# mocc

`mocc` is a memory-optimized custom container library implemented in C. It mimics the functionality of a dynamic array while focusing on strict memory management and efficient allocation strategies.

## Task Overview

The task requirement is defined in `doc/B2Task.pdf`.

This project is intended to satisfy the following high-level goals:

- Implement a dynamic array-style container in pure C (C89/C90 compatible)
- Provide initialization, insertion, deletion, random access, and resize behavior
- Optimize memory usage with a custom allocation strategy
- Handle edge cases such as out-of-bounds access and empty deletions
- Include error handling for invalid operations and memory failures
- Support testing with the Unity framework in `3rd/Unity`
- Provide a benchmark program for performance evaluation

## Prerequisites

Install the required packages on Ubuntu/Debian:

```bash
sudo apt update
sudo apt install git make gcc g++
```

If you prefer a single package set, you can also install the build essentials:

```bash
sudo apt install build-essential git
```

## Clone the Repository

Clone the project and fetch the Unity submodule in one step:

```bash
git clone --recurse-submodules https://github.com/Ahmeddsherrif/mocc
```

If you already cloned the repository without submodules, initialize them manually:

```bash
git submodule update --init --recursive
```

## Build Instructions

The repository includes a top-level `Makefile`.

To build the library and object files:

```bash
make build
```

This creates the output directory and compiles:

- `out/libmocc.a`
- `out/mocc.o`

## Run Unit Tests

This project uses Unity for unit testing. The Unity source lives in `3rd/Unity/src`.

Build and run the tests with:

```bash
make test
```

The `test` target builds `out/mocc_test` using `test/mocc_test.c` and `3rd/Unity/src/unity.c`, then executes the test binary.

## Run Benchmark

The benchmark program is built from `bench/mocc_bench.c`.

Build and run the benchmark with:

```bash
make bench
```

## Clean Build Artifacts

Remove generated output files:

```bash
make clean
```

## Notes

- Unity is used as the test framework, and its source is included under `3rd/Unity`.
- The code is compiled with AddressSanitizer enabled via `-fsanitize=address` for safer memory debugging.
- The task PDF in `doc/B2Task.pdf` describes the required dynamic array interface, advanced memory handling, concurrency, error handling, and benchmarking expectations.

## Project Layout

- `src/` — core `mocc` implementation and header
- `test/` — unit test source files
- `bench/` — benchmarking source file
- `3rd/Unity/` — Unity test framework submodule
- `doc/` — task description and documentation
