# Trick function for listing files recursively, by
# https://blog.jgc.org/2011/07/gnu-make-recursive-wildcard-function.html
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# Compile constants
CXX := g++
LINK_FLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Phony targets
.PHONY: all dev build format lint

# Default target, build and run for development
all: dev

# Dev target, builds, runs and deletes the build, for development
DEV_TARGET_DIR := .temp
DEV_BIN_NAME := app
dev:
	mkdir -p $(DEV_TARGET_DIR)
	$(CXX) $(call rwildcard,src,*.cpp) -o $(DEV_TARGET_DIR)/$(DEV_BIN_NAME) $(LINK_FLAGS)
	./$(DEV_TARGET_DIR)/$(DEV_BIN_NAME)
	rm -rf ./$(DEV_TARGET_DIR)

# Build target, builds the binary at ./build/app
BUILD_TARGET_DIR := build
BUILD_BIN_NAME := app
build:
	mkdir -p $(BUILD_TARGET_DIR)
	$(CXX) $(call rwildcard,src,*.cpp) -o $(BUILD_TARGET_DIR)/$(BUILD_BIN_NAME) $(LINK_FLAGS)

# Format files on ./src
format:
	for FILE in $(call rwildcard,src,*.h *.cpp); do clang-format -i $$FILE; done
