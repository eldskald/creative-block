# WIP

This project is me trying to learn C++ with [raylib](https://github.com/raysan5/raylib).

## Getting Started

### Dependencies

Install [raylib](https://github.com/raysan5/raylib) on your system's libraries. On Arch, a simple `sudo pacman -S raylib` will suffice, on other distros you should take look at [their page](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux). You'll also using [g++](https://wiki.archlinux.org/title/GNU_Compiler_Collection) to compile.

I'm also using [clang-format](https://clang.llvm.org/docs/ClangFormat.html) and [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) to format and lint the code, but you won't need these to run or build.

### Running and building

To run the game, either of these will work:

```console
make
make dev
```

To make a Linux build, run the following:

```console
make build
```

It will create a `build` directory and put the binary `app` in there, just run it to play the game.

Read the [Makefile](Makefile) for more details.

## License

This project is licensed under the [MIT](https://opensource.org/license/mit/) license.
