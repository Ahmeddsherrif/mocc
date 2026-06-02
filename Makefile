.SILENT:

#------------------------------------------------------------------------------
# Configuration
#------------------------------------------------------------------------------

VERBOSE_ENABLED ?= 0

CC  = gcc
CXX = g++
AR  = ar

#------------------------------------------------------------------------------
# Directories
#------------------------------------------------------------------------------

OUT_DIR   = out
SRC_DIR   = src
TEST_DIR  = test
BENCH_DIR = bench

UNITY_DIR = 3rd/Unity/src

#------------------------------------------------------------------------------
# Sources
#------------------------------------------------------------------------------

SRC_FILES = \
	$(SRC_DIR)/mocc.c \
	$(SRC_DIR)/logger.c \

SRC_HEADERS = \
	$(SRC_DIR)/mocc.h \
	$(SRC_DIR)/logger.h

SRC_OBJ = \
	$(OUT_DIR)/mocc.o \
	$(OUT_DIR)/logger.o

#------------------------------------------------------------------------------
# Include Paths
#------------------------------------------------------------------------------

INCLUDE_DIRS = \
	-I$(SRC_DIR) \
	-I$(UNITY_DIR)

#------------------------------------------------------------------------------
# Compiler Flags
#------------------------------------------------------------------------------

CFLAGS = \
	-std=c89 \
	-Wall \
	-Wextra \
	-Wpedantic \
	-g \
	-fsanitize=address \
	-pthread \
	$(INCLUDE_DIRS)

CXXFLAGS = \
	-std=c++11 \
	-Wall \
	-Wextra \
	-Wpedantic \
	-g \
	-fsanitize=address \
	-pthread \
	$(INCLUDE_DIRS)

ifeq ($(VERBOSE_ENABLED),1)
	CFLAGS   += -DVERBOSE_ENABLED
	CXXFLAGS += -DVERBOSE_ENABLED
endif

LDFLAGS = -fsanitize=address

#------------------------------------------------------------------------------
# Targets
#------------------------------------------------------------------------------

LIB       = $(OUT_DIR)/libmocc.a
TEST_EXE  = $(OUT_DIR)/mocc_test
BENCH_EXE = $(OUT_DIR)/mocc_bench

.PHONY: build clean format

#------------------------------------------------------------------------------
# Build
#------------------------------------------------------------------------------

build: format $(TEST_EXE) $(BENCH_EXE)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(LIB): $(SRC_OBJ)
	$(AR) rcs $@ $^

$(OUT_DIR)/mocc.o: $(SRC_DIR)/mocc.c $(SRC_HEADERS) | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)/logger.o: $(SRC_DIR)/logger.c $(SRC_HEADERS) | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#------------------------------------------------------------------------------
# Tests
#------------------------------------------------------------------------------

$(TEST_EXE): $(TEST_DIR)/mocc_test.c $(LIB)
	$(CC) $(CFLAGS) \
		$(TEST_DIR)/mocc_test.c \
		$(UNITY_DIR)/unity.c \
		-L$(OUT_DIR) \
		-lmocc \
		-o $@ \
		$(LDFLAGS)

#------------------------------------------------------------------------------
# Benchmarks
#------------------------------------------------------------------------------

$(BENCH_EXE): $(BENCH_DIR)/mocc_bench.c $(LIB)
	$(CXX) $(CXXFLAGS) \
		$(BENCH_DIR)/mocc_bench.c \
		-L$(OUT_DIR) \
		-lmocc \
		-o $@ \
		$(LDFLAGS)

#------------------------------------------------------------------------------
# Utilities
#------------------------------------------------------------------------------

format:
	clang-format -i --style=file \
		$(SRC_DIR)/*.c \
		$(SRC_DIR)/*.h \
		$(TEST_DIR)/*.c \
		$(BENCH_DIR)/*.c

clean:
	rm -rf $(OUT_DIR)