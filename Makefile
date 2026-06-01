#.SILENT:

CC = gcc
CXX = g++
AR = ar

OUT_DIR = out

SRC_DIR = src

TEST_DIR = test

BENCH_DIR = bench

BIN_DIR = bin

UNITY_DIR = 3rd/Unity/src

INCLUDE_DIRS = -I$(SRC_DIR) -I$(UNITY_DIR)

CFLAGS = -std=c89 \
          -Wall \
          -Wextra \
          -Wpedantic \
          -g \
          -fsanitize=address \
		  $(INCLUDE_DIRS)

CXXFLAGS = -std=c++11 \
           -Wall \
           -Wextra \
           -Wpedantic \
           -g \
           -fsanitize=address \
		   $(INCLUDE_DIRS)

LDFLAGS = -fsanitize=address

LIB = $(OUT_DIR)/libmocc.a
MOCC_OBJ = $(OUT_DIR)/mocc.o

TEST_EXE = $(OUT_DIR)/mocc_test
BENCH_EXE = $(OUT_DIR)/mocc_bench

.PHONY: build test bench clean

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

build: $(LIB)

$(LIB): $(MOCC_OBJ)
	$(AR) rcs $@ $^

$(MOCC_OBJ): $(SRC_DIR)/mocc.c $(SRC_DIR)/mocc.h | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/mocc.c -o $@

$(TEST_EXE): $(TEST_DIR)/mocc_test.c $(LIB)
	$(CC) $(CFLAGS) $(TEST_DIR)/mocc_test.c $(UNITY_DIR)/unity.c -L$(OUT_DIR) -lmocc -o $@ $(LDFLAGS)

$(BENCH_EXE): $(BENCH_DIR)/mocc_bench.c $(LIB)
	$(CXX) $(CXXFLAGS) $(BENCH_DIR)/mocc_bench.c -L$(OUT_DIR) -lmocc -o $@ $(LDFLAGS)

test: $(TEST_EXE)
	ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 ./$(TEST_EXE)

bench: $(BENCH_EXE)
	ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 ./$(BENCH_EXE)

clean:
	rm -rf $(OUT_DIR)