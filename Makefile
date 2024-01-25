# Trick function for listing files recursively, by
# https://blog.jgc.org/2011/07/gnu-make-recursive-wildcard-function.html
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# File/directory names
BIN_NAME := app
DEV_BIN_NAME := dev-app
BUILD_DIR := build
TEMP_DIR := .tmp
LIBS_DIR := lib
INCLUDE_DIR := include

# Dependencies
RAYLIB_REPO := https://github.com/raysan5/raylib.git
RAYLIB_TAG := 5.0

CONFIGS = $(file < project-settings.config)
COMPILE_FLAGS += -I./include $(foreach LINE,$(CONFIGS),-D $(LINE))
LINK_FLAGS := -L./lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
DEV_FLAGS := -D DEV=1

# Phony targets
.PHONY: all clean dev debug build format

# Default target, build and run for development
all: build

# Clean target, deletes build, temporary, include and lib folders
clean:
	rm -rf ./$(BUILD_DIR) ./$(TEMP_DIR) ./$(INCLUDE_DIR) ./$(LIBS_DIR)

# Install target, downloads, compiles and install dependencies
install:
	mkdir -p $(TEMP_DIR) $(INCLUDE_DIR) $(LIBS_DIR)
	cd $(TEMP_DIR) && git clone --depth 1 --branch $(RAYLIB_TAG) $(RAYLIB_REPO)
	cd $(TEMP_DIR)/raylib/src && make PLATFORM=PLATFORM_DESKTOP
	mv $(TEMP_DIR)/raylib/src/libraylib.a ./$(LIBS_DIR)
	mv $(TEMP_DIR)/raylib/src/raylib.h ./$(INCLUDE_DIR)
	mv $(TEMP_DIR)/raylib/src/raymath.h ./$(INCLUDE_DIR)
	rm -rf ./$(TEMP_DIR)

# Dev target, builds, runs and deletes the build, for development
dev:
	mkdir -p $(BUILD_DIR)
	g++ $(call rwildcard,src,*.cpp) -o $(BUILD_DIR)/$(DEV_BIN_NAME) $(COMPILE_FLAGS) $(DEV_FLAGS) $(LINK_FLAGS)
	./$(BUILD_DIR)/$(DEV_BIN_NAME)

# Debug target, builds a debug version, runs with the debugger and deletes the build
debug:
	mkdir -p $(TEMP_DIR)
	g++ -O0 -g $(call rwildcard,src,*.cpp) -o $(TEMP_DIR)/$(DEV_BIN_NAME) $(COMPILE_FLAGS) $(DEV_FLAGS) $(LINK_FLAGS)
	gdb $(TEMP_DIR)/$(DEV_BIN_NAME)
	rm -rf ./$(TEMP_DIR)

# Build target, builds the binary at the build target folder
build:
	mkdir -p $(BUILD_DIR)
	g++ $(call rwildcard,src,*.cpp) -o $(BUILD_DIR)/$(BIN_NAME) $(COMPILE_FLAGS) $(LINK_FLAGS)

# Format files on ./src
format:
	for FILE in $(call rwildcard,src,*.h *.cpp *.tpp); do clang-format -i $$FILE; done
