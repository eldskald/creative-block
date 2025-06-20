# Creative Block

This project is me trying to learn C++ with [raylib](https://github.com/raysan5/raylib).

## Dependencies

This project is using the GNU Toolchain for C/C++ development, in particular, the [GNU Compiler Collection](https://www.gnu.org/software/gcc), the [MinGW-w64](https://www.mingw-w64.org/) compilers and [make](https://www.gnu.org/software/make). If you want to make an HTML5 build, you need to install [emsdk](https://emscripten.org/docs/getting_started/downloads.html). See more details on below.

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
make                # All of the above
```

It will create a `build` directory and put the binary `app` in there, just run it to play the game.

To build and run a development build, run the following:

```console
make dev
```

It will create a `build` directory, put the binary `dev-app` in there and run it, deleting it after you close it. Easy for quickly compiling changes and running the game with them. You can also add a `SCENE` env var to load a specific scene on `assets/scenes` by name without the `.dat`, for example:

```console
SCENE=03 make dev
```

This will run the game starting on level 3, on `assets/scenes/03.dat` file. Only works on dev mode, that is, when running `make dev`.

You can also make a development build that won't be ran automatically or deleted after you closing by running this:

```console
make build-dev
```

This will put the build at the project root. Can be ran on a debugger and can be sent to testers to use with the built-in dev tools.

To format and lint the whole project, run the following:

```console
make format
make lint
```

Read the [Makefile](Makefile) for more details.

## Working with a web build

To make a web build, you will need to have [emsdk](https://emscripten.org/docs/getting_started/downloads.html) version 3.1.62 and `python`. Follow the link on how to install it in this version. Then, you will need to setup an emscripten config file using the `clang` and `node` binaries that come on the `emsdk` repository, that means the following lines on your `.emscripten` file:

```
LLVM_ROOT = '<your-emsdk-directory>/upstream/bin'
BINARYEN_ROOT = '<your-emsdk-directory>/upstream'
NODE_JS = '<your-emsdk-directory>/node/18.20.3_64bit/bin/node'
```

If you can't compile either `raylib` or the game, pay attention to the error messages, might be some of these binaries that can't be found. Oh, and don't forget to setup your `.env` file with the path of your installation.

After successfully compiling with `make build-web`, you can run the following python line on `build/web`:

```console
python -m http.server 3000
```

This will serve the web build folder on your `localhost:3000`. You can change the port to whatever you want.

## License

This project is licensed under the [MIT](https://opensource.org/license/mit/) license.
