# Trick function for listing files recursively, by
# https://blog.jgc.org/2011/07/gnu-make-recursive-wildcard-function.html
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# Load .env default values and then .env itself
-include .env
export

# Paths to compile raylib to web
EMSCRIPTEN_PATH := $(EMSDK_PATH)/upstream/emscripten
PATH := $(shell printenv PATH):$(EMSCRIPTEN_PATH)

# File/directory names
APP_NAME ?= app
BUILD_WEB_SHELL ?= shell.html
EDITOR_NAME ?= level-editor
TEMP_DIR := .tmp
INCLUDE_DIR := include
LIBS_DIR := lib
LINUX_LIBS := $(LIBS_DIR)/linux
WIN_LIBS := $(LIBS_DIR)/windows
WEB_LIBS := $(LIBS_DIR)/web
BUILD_DIR := build
LINUX_BUILD := $(BUILD_DIR)/linux
WIN_BUILD := $(BUILD_DIR)/windows
WEB_BUILD := $(BUILD_DIR)/web
ifeq ($(DEV_PLATFORM), Linux)
	DEV_LIBS := $(LINUX_LIBS)
endif
ifeq ($(DEV_PLATFORM), Windows)
	DEV_LIBS := $(WIN_LIBS)
endif

DEV_COMPILE_FLAGS = -std=c++17 -Wall -I./src -I./$(INCLUDE_DIR) -DDEV
DEV_LINK_FLAGS = -L./$(DEV_LIBS) -lraylib
ifeq ($(DEV_PLATFORM), Linux)
	DEV_LINK_FLAGS += -lGL -lm -lpthread -ldl -lrt -lX11
	EXT := .x86_64
endif
ifeq ($(DEV_PLATFORM), Windows)
	DEV_LINK_FLAGS += -lgdi32 -lwinmm -lpthread -static -static-libgcc -static-libstdc++
	EXT := .exe
endif
ifdef SCENE
	DEV_COMPILE_FLAGS += -DSCENE=\"assets/scenes/$(SCENE).dat\"
endif

EDITOR_COMPILE_FLAGS = -std=c++17 -Wall -I./$(INCLUDE_DIR)
EDITOR_LINK_FLAGS = $(DEV_LINK_FLAGS)

LINUX_COMPILE_FLAGS = -Wall -I./src -I./$(INCLUDE_DIR)
LINUX_LINK_FLAGS = -L./$(LINUX_LIBS) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
WIN_COMPILE_FLAGS = -Wall -I./src -I./$(INCLUDE_DIR)
WIN_LINK_FLAGS = -L./$(WIN_LIBS) -lraylib -lgdi32 -lwinmm -lpthread -static -static-libgcc -static-libstdc++
WEB_COMPILE_FLAGS = -Os -Wall -I./src -I./$(INCLUDE_DIR) -I$(EMSCRIPTEN_PATH)/cache/sysroot/include --preload-file assets --shell-file $(BUILD_WEB_SHELL) -DWEB
WEB_LINK_FLAGS = -L./$(WEB_LIBS) -lraylib -s USE_GLFW=3 -s TOTAL_MEMORY=$(BUILD_WEB_HEAP_SIZE) -s FORCE_FILESYSTEM=1

# Phony targets
.PHONY: all clean install dev debug build-linux build-windows build-web editor debug-editor format lint

# Default target, cleans and build for all platforms
all: build-linux build-windows build-web

# Clean target, deletes build, temporary, include and lib folders
clean:
	rm -rf ./$(BUILD_DIR) ./$(TEMP_DIR) ./$(INCLUDE_DIR) ./$(LIBS_DIR)

# Install target, clones raylib and install linux, windows and web versions
install:
	-rm -rf ./$(INCLUDE_DIR) ./$(LIBS_DIR)
	-mkdir -p $(TEMP_DIR) $(INCLUDE_DIR) $(LIBS_DIR) $(LINUX_LIBS) $(WIN_LIBS) $(WEB_LIBS)
	-cd $(TEMP_DIR) && git clone --depth 1 --branch 5.5 https://github.com/raysan5/raylib.git
	-cd $(TEMP_DIR)/raylib/src && make PLATFORM=PLATFORM_DESKTOP
	-cp $(TEMP_DIR)/raylib/src/raylib.h ./$(INCLUDE_DIR)
	-cp $(TEMP_DIR)/raylib/src/raymath.h ./$(INCLUDE_DIR)
	-mv $(TEMP_DIR)/raylib/src/libraylib.a ./$(LINUX_LIBS)
	-rm -rf ./$(TEMP_DIR)
	-mkdir -p $(TEMP_DIR)
	-cd $(TEMP_DIR) && git clone --depth 1 --branch 5.5 https://github.com/raysan5/raylib.git
	-cd $(TEMP_DIR)/raylib/src && make PLATFORM=PLATFORM_DESKTOP OS=Windows_NT CC=x86_64-w64-mingw32-gcc AR=x86_64-w64-mingw32-ar
	-mv $(TEMP_DIR)/raylib/src/libraylib.a ./$(WIN_LIBS)
	-rm -rf ./$(TEMP_DIR)
	-mkdir -p $(TEMP_DIR)
	-cd $(TEMP_DIR) && git clone --depth 1 --branch 5.5 https://github.com/raysan5/raylib.git
	-cd $(TEMP_DIR)/raylib/src && make PLATFORM=PLATFORM_WEB -B
	-mv $(TEMP_DIR)/raylib/src/libraylib.a ./$(WEB_LIBS)
	rm -rf ./$(TEMP_DIR)

# Dev target, builds, runs and deletes the build, for development
dev:
	-mkdir -p $(TEMP_DIR)
	-$(CC) $(call rwildcard,src,*.cpp) -o $(TEMP_DIR)/$(APP_NAME)$(EXT) $(DEV_COMPILE_FLAGS) $(DEV_LINK_FLAGS)
	-./$(TEMP_DIR)/$(APP_NAME)$(EXT)
	rm -rf ./$(TEMP_DIR)

# Debug target, builds a debug version, runs with the debugger and deletes the build
debug:
	-mkdir -p $(TEMP_DIR)
	-$(CC) -O0 -g $(call rwildcard,src,*.cpp) -o $(TEMP_DIR)/$(APP_NAME)$(EXT) $(DEV_COMPILE_FLAGS) $(DEV_LINK_FLAGS)
	-$(GDB) -tui $(TEMP_DIR)/$(APP_NAME)$(EXT)
	rm -rf ./$(TEMP_DIR)

# Build for the Linux platform, puts the binary at the build target folder
build-linux:
	-rm -r $(LINUX_BUILD)
	mkdir -p $(LINUX_BUILD)
	g++ $(call rwildcard,src,*.cpp) -o $(LINUX_BUILD)/$(APP_NAME).x64_86 $(LINUX_COMPILE_FLAGS) $(LINUX_LINK_FLAGS)
	cp -r assets $(LINUX_BUILD)

# Build for the Windows platform, puts the binary at the build target folder
build-windows:
	-rm -r $(WIN_BUILD)
	mkdir -p $(WIN_BUILD)
	x86_64-w64-mingw32-g++ $(call rwildcard,src,*.cpp) -o $(WIN_BUILD)/$(APP_NAME).exe $(WIN_COMPILE_FLAGS) $(WIN_LINK_FLAGS)
	cp -r assets $(WIN_BUILD)

# Build for Web
build-web:
	-rm -r $(WEB_BUILD)
	mkdir -p $(WEB_BUILD)
	emcc $(call rwildcard,src,*.cpp) -o $(WEB_BUILD)/$(APP_NAME).html $(WEB_COMPILE_FLAGS) $(WEB_LINK_FLAGS)

# Editor target, compiles the level editor and places it at the project root
editor:
	$(CC) $(call rwildcard,editor,*.cpp) -o $(EDITOR_NAME)$(EXT) $(EDITOR_COMPILE_FLAGS) $(EDITOR_LINK_FLAGS)

# Debug editor target, builds a debug version of the editor, runs with the
# debugger and deletes the build
debug-editor:
	-mkdir -p $(TEMP_DIR)
	-$(CC) -O0 -g $(call rwildcard,editor,*.cpp) -o $(TEMP_DIR)/$(EDITOR_NAME)$(EXT) $(EDITOR_COMPILE_FLAGS) $(EDITOR_LINK_FLAGS)
	-$(GDB) -tui $(TEMP_DIR)/$(EDITOR_NAME)$(EXT)
	rm -rf ./$(TEMP_DIR)

# Format files on ./src
format:
	for FILE in $(call rwildcard,src,*.h *.cpp *.tpp); do clang-format -i $$FILE; done
	for FILE in $(call rwildcard,editor,*.h *.cpp *.tpp); do clang-format -i $$FILE; done

# Lint files on ./src
lint:
	for FILE in $(call rwildcard,src,*.h *.cpp *.tpp); do clang-tidy $$FILE; done
	for FILE in $(call rwildcard,editor,*.h *.cpp *.tpp); do clang-tidy $$FILE; done
