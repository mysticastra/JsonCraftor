# JsonCraftor Makefile
# Author: Xentixar
# Version: 1.0.0

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -std=c11
INCLUDES := -I.

# Directories
BUILD_DIR := build
EXAMPLE_DIR := examples
TEST_DIR := tests

# Source files
EXAMPLE_SRC := $(EXAMPLE_DIR)/example.c
TEST_SRC := $(TEST_DIR)/test_parser.c

# Output binaries
EXAMPLE_BIN := $(BUILD_DIR)/example
TEST_BIN := $(BUILD_DIR)/test_parser

# Colors for pretty output
GREEN := \033[0;32m
RED := \033[0;31m
YELLOW := \033[0;33m
NC := \033[0m # No Color

# Version
VERSION := 1.0.0

.DEFAULT_GOAL := help

.PHONY: all clean test run help version dirs

# Help target
help:
	@echo "$(YELLOW)JsonCraftor$(NC) - JSON to C Struct Parser"
	@echo "Version: $(VERSION)"
	@echo ""
	@echo "$(YELLOW)Available targets:$(NC)"
	@echo "  make all         - Build both example and tests"
	@echo "  make example     - Build only the example"
	@echo "  make test        - Build and run tests"
	@echo "  make run         - Build and run the example"
	@echo "  make clean       - Remove all built files"
	@echo "  make version     - Display version information"
	@echo "  make help        - Display this help message"
	@echo ""
	@echo "$(YELLOW)Development targets:$(NC)"
	@echo "  make debug       - Build with debug symbols"
	@echo "  make check       - Run static analysis"
	@echo ""
	@echo "$(YELLOW)Example usage:$(NC)"
	@echo "  make test        # Run all tests"
	@echo "  make run         # Run the example"

# Version information
version:
	@echo "$(YELLOW)JsonCraftor$(NC) version $(VERSION)"
	@echo "Build with $(CC) $(CFLAGS)"

# Create build directories
dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(EXAMPLE_DIR)
	@mkdir -p $(BUILD_DIR)/$(TEST_DIR)

# Build all targets
all: dirs $(EXAMPLE_BIN) $(TEST_BIN)
	@echo "$(GREEN)Build complete!$(NC)"

# Build example
$(EXAMPLE_BIN): $(EXAMPLE_SRC) jsonparser.h
	@echo "$(YELLOW)Building example...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<
	@echo "$(GREEN)Example built successfully!$(NC)"

example: dirs $(EXAMPLE_BIN)

# Build tests
$(TEST_BIN): $(TEST_SRC) jsonparser.h
	@echo "$(YELLOW)Building tests...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<
	@echo "$(GREEN)Tests built successfully!$(NC)"

# Run tests
test: dirs $(TEST_BIN)
	@echo "$(YELLOW)Running tests...$(NC)"
	@./$(TEST_BIN)

# Run example
run: dirs $(EXAMPLE_BIN)
	@echo "$(YELLOW)Running example...$(NC)"
	@./$(EXAMPLE_BIN)

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: all
	@echo "$(GREEN)Debug build complete!$(NC)"

# Static analysis
check:
	@echo "$(YELLOW)Running static analysis...$(NC)"
	@if command -v cppcheck >/dev/null; then \
		cppcheck --enable=all --suppress=missingIncludeSystem . ; \
	else \
		echo "$(RED)cppcheck not found. Please install cppcheck.$(NC)"; \
	fi

# Clean up
clean:
	@echo "$(YELLOW)Cleaning up...$(NC)"
	@rm -rf $(BUILD_DIR)
	@echo "$(GREEN)Cleanup complete!$(NC)"

# Error handling
.SILENT: error
error:
	echo "$(RED)Error: $@$(NC)" >&2
	exit 1 