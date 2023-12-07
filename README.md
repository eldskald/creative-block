# WIP

This project is me trying to learn C++ with [raylib](https://github.com/raysan5/raylib).

## Getting Started

### Dependencies

Install [raylib](https://github.com/raysan5/raylib) on your system's libraries. On Arch, a simple `sudo pacman -S raylib` will suffice, on other distros you should take look at [their page](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux). I'm also using [g++](https://wiki.archlinux.org/title/GNU_Compiler_Collection) to compile, so you'll need it to build. There is also a debug command on the [Makefile](Makefile) using [gdb](https://www.sourceware.org/gdb/), you don't need this to build but you do if you want to debug.

I'm also using [clang-format](https://clang.llvm.org/docs/ClangFormat.html) to format the code, but you won't need it to run or build.

### Running and building

To make a Linux build, run one of these two commands:

```console
make
make build
```

It will create a `build` directory and put the binary `app` in there, just run it to play the game.

To build and run a development build, run the following:

```console
make dev
```

It will create a `build` directory, put the binary `dev-app` in there and run it. Easy for quickly compiling changes and running the game with them.

To run the debugger, run the following:

```console
make debug
```

It compiles a development build with debug info, runs [gdb](https://www.sourceware.org/gdb/) on it and after you're done, it deletes it.

To format the whole project, run the following:

```console
make format
```

Read the [Makefile](Makefile) for more details.

## License

This project is licensed under the [MIT](https://opensource.org/license/mit/) license.
