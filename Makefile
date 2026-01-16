# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2
DEBUG_CFLAGS = -Wall -Wextra -std=c99 -pedantic -g -O0
LDFLAGS =

# Directories
SRC_DIR = .
TEST_DIR = tests
BUILD_DIR = build
LIB_DIR = $(BUILD_DIR)/lib
BIN_DIR = $(BUILD_DIR)/bin
TEST_BIN_DIR = $(BUILD_DIR)/tests

# Source files
LIB_SRC = $(SRC_DIR)/libtct.c
LIB_HDR = $(SRC_DIR)/libtct.h
EXAMPLE_SRC = $(SRC_DIR)/example.c

# Test files
TEST_SRCS = $(wildcard $(TEST_DIR)/test_*.c)
TEST_BINS = $(patsubst $(TEST_DIR)/%.c,$(TEST_BIN_DIR)/%,$(TEST_SRCS))

# Library output
LIB_STATIC = $(LIB_DIR)/libtct.a
LIB_OBJ = $(BUILD_DIR)/libtct.o

# Example binary
EXAMPLE_BIN = $(BIN_DIR)/example

# Targets
.PHONY: all clean test install help dirs

all: dirs $(LIB_STATIC) $(EXAMPLE_BIN)

# Create necessary directories
dirs:
	@mkdir -p $(BUILD_DIR) $(LIB_DIR) $(BIN_DIR) $(TEST_BIN_DIR)

# Build static library
$(LIB_STATIC): $(LIB_OBJ)
	@echo "Creating static library: $@"
	ar rcs $@ $^

$(LIB_OBJ): $(LIB_SRC) $(LIB_HDR)
	@echo "Compiling library object: $@"
	$(CC) $(CFLAGS) -c $(LIB_SRC) -o $@

# Build example
$(EXAMPLE_BIN): $(EXAMPLE_SRC) $(LIB_STATIC)
	@echo "Building example: $@"
	$(CC) $(CFLAGS) $(EXAMPLE_SRC) -o $@ -L$(LIB_DIR) -ltct

# Build tests
$(TEST_BIN_DIR)/%: $(TEST_DIR)/%.c $(LIB_STATIC)
	@echo "Building test: $@"
	$(CC) $(CFLAGS) $< -o $@ -L$(LIB_DIR) -ltct

# Run tests
test: dirs $(LIB_STATIC) $(TEST_BINS)
	@echo "=========================================="
	@echo "Running test suite"
	@echo "=========================================="
	@failed=0; \
	for test in $(TEST_BINS); do \
		echo ""; \
		$$test || failed=$$((failed + 1)); \
	done; \
	echo ""; \
	echo "=========================================="; \
	if [ $$failed -eq 0 ]; then \
		echo "All test suites PASSED!"; \
		echo "=========================================="; \
	else \
		echo "$$failed test suite(s) FAILED!"; \
		echo "=========================================="; \
		exit 1; \
	fi

# Run example
run: $(EXAMPLE_BIN)
	@echo "Running example:"
	@echo "=========================================="
	@$(EXAMPLE_BIN)
	@echo "=========================================="

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR)

# Install library (optional, requires root/sudo)
install: $(LIB_STATIC) $(LIB_HDR)
	@echo "Installing library to /usr/local..."
	install -d /usr/local/lib
	install -d /usr/local/include
	install -m 644 $(LIB_STATIC) /usr/local/lib/
	install -m 644 $(LIB_HDR) /usr/local/include/
	@echo "Installation complete!"

# Uninstall library
uninstall:
	@echo "Uninstalling library from /usr/local..."
	rm -f /usr/local/lib/libtct.a
	rm -f /usr/local/include/libtct.h
	@echo "Uninstallation complete!"

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build library and example (default)"
	@echo "  test      - Build and run all tests"
	@echo "  run       - Build and run the example"
	@echo "  clean     - Remove all build artifacts"
	@echo "  install   - Install library to /usr/local (requires sudo)"
	@echo "  uninstall - Remove library from /usr/local (requires sudo)"
	@echo "  help      - Show this help message"
