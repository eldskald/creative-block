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
APP_NAME 		?= creative-block
BUILD_WEB_SHELL ?= shell.html
EDITOR_NAME 	?= level-editor
INCLUDE_DIR 	:= include
LIBS_DIR 		:= lib
LINUX_LIBS 		:= $(LIBS_DIR)/linux
WIN_LIBS 		:= $(LIBS_DIR)/windows
WEB_LIBS 		:= $(LIBS_DIR)/web
BUILD_DIR 		:= build
LINUX_BUILD 	:= $(BUILD_DIR)/linux
WIN_BUILD 		:= $(BUILD_DIR)/windows
WEB_BUILD 		:= $(BUILD_DIR)/web

ifeq ($(DEV_PLATFORM), Linux)
	DEV_LIBS := $(LINUX_LIBS)
endif
ifeq ($(DEV_PLATFORM), Windows)
	DEV_LIBS := $(WIN_LIBS)
endif

DEV_COMPILE_FLAGS = -std=c++17 -Wall -I./src -I./$(INCLUDE_DIR) -DDEV -fsanitize=address
DEV_LINK_FLAGS = -L./$(DEV_LIBS)
ifeq ($(DEV_PLATFORM), Linux)
	DEV_LINK_FLAGS += -lraylib -lSDL2 -lSDL2main -lGL -lm -lpthread -ldl -lrt -lX11
	EXT := .x86_64
endif
ifeq ($(DEV_PLATFORM), Windows)
	DEV_LINK_FLAGS += -lgdi32 -lwinmm -lpthread -static -static-libgcc -static-libstdc++
	EXT := .exe
endif
ifdef SCENE
	DEV_COMPILE_FLAGS += -DSCENE=$(SCENE)
endif

EDITOR_COMPILE_FLAGS = -std=c++17 -Wall -I./$(INCLUDE_DIR)
EDITOR_LINK_FLAGS = $(DEV_LINK_FLAGS)

LINUX_COMPILE_FLAGS = -Wall -I./src -I./$(INCLUDE_DIR)
LINUX_LINK_FLAGS = -L./$(LINUX_LIBS) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lSDL2 -lSDL2main
WIN_COMPILE_FLAGS = -Wall -I./src -I./$(INCLUDE_DIR)
WIN_LINK_FLAGS = -L./$(WIN_LIBS) -lraylib -lgdi32 -lwinmm -lpthread -static -static-libgcc -static-libstdc++
WEB_COMPILE_FLAGS = -Os -Wall -I./src -I./$(INCLUDE_DIR) -I$(EMSCRIPTEN_PATH)/cache/sysroot/include --preload-file assets --shell-file $(BUILD_WEB_SHELL) -DWEB -s USE_GLFW=3 -s EXPORTED_RUNTIME_METHODS=HEAPF32,requestFullscreen
WEB_LINK_FLAGS = -L./$(WEB_LIBS) -lraylib

# Phony targets
.PHONY: all clean dev web-dev linux windows web editor

# Default target, builds for all platforms
all: linux windows web

# Clean target, deletes builds and libraries
clean:
	rm -rf ./$(BUILD_DIR) ./$(INCLUDE_DIR) ./$(LIBS_DIR)

# Build a development build, can be ran on the compiler or sent to others with development tools
dev:
	-$(CC) -O0 -g $(call rwildcard,src,*.cpp) -o $(APP_NAME)$(EXT) $(DEV_COMPILE_FLAGS) $(DEV_LINK_FLAGS)

# Build a development build for web. It's web build with dev flags for logging and debugging
web-dev:
	-rm -r $(WEB_BUILD)
	mkdir -p $(WEB_BUILD)
	emcc $(call rwildcard,src,*.cpp) -o $(WEB_BUILD)/$(APP_NAME).html $(WEB_COMPILE_FLAGS) $(WEB_LINK_FLAGS) -DDEV -s ASSERTIONS=1


# Build for the Linux platform, puts the binary at the build target folder
linux:
	-rm -r $(LINUX_BUILD)
	mkdir -p $(LINUX_BUILD)
	g++ $(call rwildcard,src,*.cpp) -o $(LINUX_BUILD)/$(APP_NAME).x64_86 $(LINUX_COMPILE_FLAGS) $(LINUX_LINK_FLAGS)
	cp -r assets $(LINUX_BUILD)

# Build for the Windows platform, puts the binary at the build target folder
windows:
	-rm -r $(WIN_BUILD)
	mkdir -p $(WIN_BUILD)
	x86_64-w64-mingw32-g++ $(call rwildcard,src,*.cpp) -o $(WIN_BUILD)/$(APP_NAME).exe $(WIN_COMPILE_FLAGS) $(WIN_LINK_FLAGS)
	cp -r assets $(WIN_BUILD)

# Build for Web
web:
	-rm -r $(WEB_BUILD)
	mkdir -p $(WEB_BUILD)
	emcc $(call rwildcard,src,*.cpp) -o $(WEB_BUILD)/$(APP_NAME).html $(WEB_COMPILE_FLAGS) $(WEB_LINK_FLAGS)

# Editor target, compiles the level editor and places it at the project root. Has debugger info.
editor:
	$(CC) -O0 -g $(call rwildcard,editor,*.cpp) -o $(EDITOR_NAME)$(EXT) $(EDITOR_COMPILE_FLAGS) $(EDITOR_LINK_FLAGS)
