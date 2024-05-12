# Trick function for listing files recursively, by
# https://blog.jgc.org/2011/07/gnu-make-recursive-wildcard-function.html
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# Load .env default values and then .env itself
DEV_PLATFORM = Linux
CC = g++
GDB = gdb
EMSDK_PATH = $(HOME)/emsdk
-include .env
export

# Paths to compile raylib to web
EMSCRIPTEN_PATH = $(EMSDK_PATH)/upstream/emscripten
CLANG_PATH = $(EMSDK_PATH)/upstream/bin
PYTHON_PATH = $(PATH)

# File/directory names
BIN_NAME := app
DEV_BIN_NAME := dev-app
EDITOR_NAME := level-editor
BUILD_DIR := build
TEMP_DIR := .tmp
LIBS_DIR := lib
INCLUDE_DIR := include

# Compiler flags
SETTINGS = $(file < settings.cfg)
IMPORTS = $(file < imports.cfg)
INSTALL_VARS = PLATFORM=PLATFORM_DESKTOP
DEV_COMPILE_FLAGS = -Wall -I./$(INCLUDE_DIR) $(foreach LINE,$(SETTINGS),-D$(LINE)) $(foreach LINE,$(IMPORTS),-D$(LINE))
DEV_LINK_FLAGS = -L./$(LIBS_DIR) -lraylib
LINUX_COMPILE_FLAGS = -Wall -I./$(TEMP_DIR)/$(INCLUDE_DIR) $(foreach LINE,$(SETTINGS),-D$(LINE)) $(foreach LINE,$(IMPORTS),-D$(LINE))
LINUX_LINK_FLAGS = -L./$(TEMP_DIR)/$(LIBS_DIR) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
WIN_COMPILE_FLAGS = -Wall -I./$(TEMP_DIR)/$(INCLUDE_DIR) $(foreach LINE,$(SETTINGS),-D$(LINE)) $(foreach LINE,$(IMPORTS),-D$(LINE))
WIN_LINK_FLAGS = -L./$(TEMP_DIR)/$(LIBS_DIR) -lraylib -lgdi32 -lwinmm -lpthread -static -static-libgcc -static-libstdc++
WEB_COMPILE_FLAGS = -Os -Wall -I./$(TEMP_DIR)/$(INCLUDE_DIR) -I$(EMSCRIPTEN_PATH)/cache/sysroot/include $(foreach LINE,$(SETTINGS),-D$(LINE)) $(foreach LINE,$(IMPORTS),-D$(LINE)) --preload-file assets -DPLATFORM_WEB
WEB_LINK_FLAGS = -L./$(TEMP_DIR)/$(LIBS_DIR) -lraylib -s USE_GLFW=3 -s
DEV_FLAGS = -DDEV=1
ifeq ($(DEV_PLATFORM), Linux)
	DEV_LINK_FLAGS += -lGL -lm -lpthread -ldl -lrt -lX11
	DEV_BIN_NAME := $(DEV_BIN_NAME).x86_64
endif
ifeq ($(DEV_PLATFORM), Windows)
	DEV_LINK_FLAGS += -lgdi32 -lwinmm -lpthread -static -static-libgcc -static-libstdc++
	DEV_BIN_NAME := $(DEV_BIN_NAME).exe
	INSTALL_VARS += OS=Windows_NT CC=x86_64-w64-mingw32-gcc AR=x86_64-w64-mingw32-ar
endif

# Phony targets
.PHONY: all clean install dev debug build-linux build-windows build-web editor format lint

# Default target, cleans and build for all platforms
all: build-linux build-windows build-web

# Clean target, deletes build, temporary, include and lib folders
clean:
	rm -rf ./$(BUILD_DIR) ./$(TEMP_DIR) ./$(INCLUDE_DIR) ./$(LIBS_DIR)

# Install target, downloads, compiles and install dependencies
install:
	mkdir -p $(TEMP_DIR) $(INCLUDE_DIR) $(LIBS_DIR)
	cd $(TEMP_DIR) && git clone --depth 1 --branch 5.0 https://github.com/raysan5/raylib.git
	cd $(TEMP_DIR)/raylib/src && make $(INSTALL_VARS)
	mv $(TEMP_DIR)/raylib/src/libraylib.a ./$(LIBS_DIR)
	mv $(TEMP_DIR)/raylib/src/raylib.h ./$(INCLUDE_DIR)
	mv $(TEMP_DIR)/raylib/src/raymath.h ./$(INCLUDE_DIR)
	rm -rf ./$(TEMP_DIR)

# Dev target, builds, runs and deletes the build, for development
dev:
	mkdir -p $(BUILD_DIR)
	$(CC) $(call rwildcard,src,*.cpp) -o $(BUILD_DIR)/$(DEV_BIN_NAME) $(DEV_COMPILE_FLAGS) $(DEV_FLAGS) $(DEV_LINK_FLAGS)
	./$(BUILD_DIR)/$(DEV_BIN_NAME)

# Debug target, builds a debug version, runs with the debugger and deletes the build
debug:
	mkdir -p $(TEMP_DIR)
	$(CC) -O0 -g $(call rwildcard,src,*.cpp) -o $(TEMP_DIR)/$(DEV_BIN_NAME) $(DEV_COMPILE_FLAGS) $(DEV_FLAGS) $(DEV_LINK_FLAGS)
	$(GDB) -tui $(TEMP_DIR)/$(DEV_BIN_NAME)
	rm -rf ./$(TEMP_DIR)

# Build for the Linux platform, puts the binary at the build target folder
build-linux:
	rm -r $(BUILD_DIR)/linux
	mkdir -p $(TEMP_DIR) $(BUILD_DIR)/linux
	cd $(TEMP_DIR) && mkdir -p $(INCLUDE_DIR) $(LIBS_DIR)
	cd $(TEMP_DIR) && git clone --depth 1 --branch 5.0 https://github.com/raysan5/raylib.git
	cd $(TEMP_DIR)/raylib/src && make PLATFORM=PLATFORM_DESKTOP
	mv $(TEMP_DIR)/raylib/src/libraylib.a $(TEMP_DIR)/$(LIBS_DIR)
	mv $(TEMP_DIR)/raylib/src/raylib.h $(TEMP_DIR)/$(INCLUDE_DIR)
	mv $(TEMP_DIR)/raylib/src/raymath.h $(TEMP_DIR)/$(INCLUDE_DIR)
	g++ $(call rwildcard,src,*.cpp) -o $(BUILD_DIR)/linux/$(BIN_NAME).x64_86 $(LINUX_COMPILE_FLAGS) $(LINUX_LINK_FLAGS)
	cp -r assets $(BUILD_DIR)/linux/
	rm -rf ./$(TEMP_DIR)

# Build for the Windows platform, puts the binary at the build target folder
build-windows:
	rm -r $(BUILD_DIR)/windows
	mkdir -p $(TEMP_DIR) $(BUILD_DIR)/windows
	cd $(TEMP_DIR) && mkdir -p $(INCLUDE_DIR) $(LIBS_DIR)
	cd $(TEMP_DIR) && git clone --depth 1 --branch 5.0 https://github.com/raysan5/raylib.git
	cd $(TEMP_DIR)/raylib/src && make PLATFORM=PLATFORM_DESKTOP OS=Windows_NT CC=x86_64-w64-mingw32-gcc AR=x86_64-w64-mingw32-ar
	mv $(TEMP_DIR)/raylib/src/libraylib.a $(TEMP_DIR)/$(LIBS_DIR)
	mv $(TEMP_DIR)/raylib/src/raylib.h $(TEMP_DIR)/$(INCLUDE_DIR)
	mv $(TEMP_DIR)/raylib/src/raymath.h $(TEMP_DIR)/$(INCLUDE_DIR)
	x86_64-w64-mingw32-g++ $(call rwildcard,src,*.cpp) -o $(BUILD_DIR)/windows/$(BIN_NAME).exe $(WIN_COMPILE_FLAGS) $(WIN_LINK_FLAGS)
	cp -r assets $(BUILD_DIR)/windows/
	rm -rf ./$(TEMP_DIR)

# Build for Web
build-web:
	rm -r $(BUILD_DIR)/web
	mkdir -p $(TEMP_DIR) $(BUILD_DIR)/web
	cd $(TEMP_DIR) && mkdir -p $(INCLUDE_DIR) $(LIBS_DIR)
	cd $(TEMP_DIR) && git clone --depth 1 --branch 5.0 https://github.com/raysan5/raylib.git
	cd $(TEMP_DIR)/raylib/src && make PLATFORM=PLATFORM_WEB -B
	mv $(TEMP_DIR)/raylib/src/libraylib.a $(TEMP_DIR)/$(LIBS_DIR)
	mv $(TEMP_DIR)/raylib/src/raylib.h $(TEMP_DIR)/$(INCLUDE_DIR)
	mv $(TEMP_DIR)/raylib/src/raymath.h $(TEMP_DIR)/$(INCLUDE_DIR)
	emcc $(call rwildcard,src,*.cpp) -o $(BUILD_DIR)/web/$(BIN_NAME).html $(WEB_COMPILE_FLAGS) $(WEB_LINK_FLAGS)
	rm -rf ./$(TEMP_DIR)

# Editor target, compiles the level editor and places it at the project root
editor:
	$(CC) $(call rwildcard,editor,*.cpp) -o $(EDITOR_NAME) $(DEV_COMPILE_FLAGS) $(DEV_LINK_FLAGS)

# Format files on ./src
format:
	for FILE in $(call rwildcard,src,*.h *.cpp *.tpp); do clang-format -i $$FILE; done

# Lint files on ./src
lint:
	for FILE in $(call rwildcard,src,*.h *.cpp *.tpp); do clang-tidy $$FILE; done

test:
	echo $(DEV_BIN_NAME)
