# WIP

This project is me trying to learn C++ with [raylib](https://github.com/raysan5/raylib).

## Dependencies

This project is using the GNU Toolchain for C/C++ development, in particular, the [GNU Compiler Collection](https://www.gnu.org/software/gcc), the [MinGW-w64](https://www.mingw-w64.org/) compilers and [make](https://www.gnu.org/software/make). Optionally, [gdb](https://www.sourceware.org/gdb/) to debug. You also need [git](https://git-scm.com/), of course.

You'll also need basic libraries used by [raylib](https://github.com/raysan5/raylib) to do graphics and audio. More info on their [wiki](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).

It's also using [clang-format](https://clang.llvm.org/docs/ClangFormat.html) to format the code, as well as [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) to lint.

## Getting started

After cloning the repository, setup your `.env` file by copying the [.env.example](.env.example) and renaming it to `.env`, and then setting the values. The default values are for working on Linux. If you are working on Windows, use WSL.

Then, run the following to install dependencies:

```console
make install
```

This will install [raylib](https://github.com/raysan5/raylib) on this project so you can compile it. To erase dependencies and builds in case you want to redo everything, run:

```console
make clean
```

To build the level editor and edit levels, run the following:

```console
make editor
```

It will compile the binary `level-editor` at the project root, just run and start making levels. You can also drag and drop a level file at the editor and it will load it. Just press save to save it.

## Running and building

To make a build, run one of these commands:

```console
make build-linux    # Makes a Linux build
make build-windows  # Makes a Windows build
make build-web      # Makes a Web (HTML5) build
make build          # All of the above
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
