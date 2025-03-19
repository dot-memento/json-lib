CC = gcc
CFLAGS = -std=c11 -pedantic -Og -g\
	-Wall -Wextra\
	-Wcast-align\
	-Wcast-qual\
	-Wformat -Wformat=2\
	-Wimplicit-fallthrough\
	-Wincompatible-pointer-types\
	-Wpointer-arith\
	-Wshadow\
	-Wwrite-strings
LDFLAGS = -flto

# Source and object directories
SRC_DIR = src
TEST_DIR = tests
OBJ_DIR = .obj
BIN_DIR = bin

# Library sources and objects
LIB_SRCS = $(wildcard $(SRC_DIR)/*.c)
LIB_OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(LIB_SRCS))
LIB_HEADERS = $(SRC_DIR)/json.h

# Test sources and objects
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.o,$(TEST_SRCS))
TEST_BINS = $(patsubst $(TEST_DIR)/%.c,$(BIN_DIR)/%,$(TEST_SRCS))

# Default target
all: directories $(LIB_OBJS) $(TEST_BINS)

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# Compile library sources
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(LIB_HEADERS)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

# Compile test sources
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c $(LIB_HEADERS)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

# Link test binary
$(BIN_DIR)/%: $(OBJ_DIR)/%.o $(LIB_OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

# Run tests
test: $(TEST_BINS)
	@echo "Running tests..."
	@$(BIN_DIR)/test_json

# Clean up
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build library and tests (default)"
	@echo "  test      - Run tests"
	@echo "  clean     - Remove object files and binaries"
	@echo "  help      - Display this help message"

.PHONY: all test clean help directories