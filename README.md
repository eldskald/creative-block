# WIP

This project is me trying to learn C++ with [raylib](https://github.com/raysan5/raylib).

## Getting Started

### Dependencies

This project is using the GNU Toolchain for C/C++ development, in particular, the [GNU Compiler Collection](https://www.gnu.org/software/gcc) and [make](https://www.gnu.org/software/make) are used to compile, and optionally, [gdb](https://www.sourceware.org/gdb/) to debug. You also need [git](https://git-scm.com/), of course.

You'll also need basic libraries used by [raylib](https://github.com/raysan5/raylib) to do graphics and audio. More info on their [wiki](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).

It's also using [clang-format](https://clang.llvm.org/docs/ClangFormat.html) to format the code, as well as [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) to lint.

### Getting started

After cloning the repository, run the following to install dependendies:

```console
make install
```

This will install [raylib](https://github.com/raysan5/raylib) on this project so you can compile it. To erase dependencies and builds in case you want to redo everything, run:

```console
make clean
```

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

To format and lint the whole project, run the following:

```console
make format
make lint
```

Read the [Makefile](Makefile) for more details.

## License

This project is licensed under the [MIT](https://opensource.org/license/mit/) license.
