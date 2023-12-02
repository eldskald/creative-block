# Trick function for listing files recursively, by
# https://blog.jgc.org/2011/07/gnu-make-recursive-wildcard-function.html
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# Compile constants
BIN_NAME := app
BUILD_TARGET_DIR := build
TEMP_TARGET_DIR := .temp
LINK_FLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Phony targets
.PHONY: all dev debug build format

# Default target, build and run for development
all: dev

# Dev target, builds, runs and deletes the build, for development
dev:
	mkdir -p $(TEMP_TARGET_DIR)
	g++ $(call rwildcard,src,*.cpp) -o $(TEMP_TARGET_DIR)/$(BIN_NAME) $(LINK_FLAGS)
	./$(DEV_TARGET_DIR)/$(DEV_BIN_NAME)
	rm -rf ./$(TEMP_TARGET_DIR)

# Debug target, builds a debug version, runs with the debugger and deletes the build
debug:
	mkdir -p $(TEMP_TARGET_DIR)
	g++ -O0 -g $(call rwildcard,src,*.cpp) -o $(TEMP_TARGET_DIR)/$(BIN_NAME) $(LINK_FLAGS)
	gdb $(TEMP_TARGET_DIR)/$(BIN_NAME)
	rm -rf ./$(TEMP_TARGET_DIR)

# Build target, builds the binary at the build target folder
build:
	mkdir -p $(BUILD_TARGET_DIR)
	g++ $(call rwildcard,src,*.cpp) -o $(BUILD_TARGET_DIR)/$(BIN_NAME) $(LINK_FLAGS)

# Format files on ./src
format:
	for FILE in $(call rwildcard,src,*.h *.cpp); do clang-format -i $$FILE; done
