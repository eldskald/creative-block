# Creative Block

This project is me trying to learn C++ with [raylib](https://github.com/raysan5/raylib).

You can play the game or download it on [eldskald.itch.io/creative-block](https://eldskald.itch.io/creative-block).

## Dependencies

This project is using the GNU Toolchain for C/C++ development, in particular, the [GNU Compiler Collection](https://www.gnu.org/software/gcc), the [MinGW-w64](https://www.mingw-w64.org/) compilers and [make](https://www.gnu.org/software/make). If you want to make an HTML5 build, you need to install [emsdk](https://emscripten.org/docs/getting_started/downloads.html). See more details on below.

You'll also need basic libraries used by [raylib](https://github.com/raysan5/raylib) to do graphics and audio. More info on their [wiki](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).

It's also using [clang-format](https://clang.llvm.org/docs/ClangFormat.html) to format the code, as well as [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) to lint.

## Getting started

After cloning the repository, setup your `.env` file by copying the [.env.example](.env.example) and renaming it to `.env`, and then setting the values. The default values are for working on Linux. If you are working on Windows, use WSL.

Then, run the following to install dependencies:

```console
bin/install-dependencies
```

This will install [raylib](https://github.com/raysan5/raylib) on this project so you can compile it. You need [emsdk](https://emscripten.org/docs/getting_started/downloads.html) and its path on `.env` to compile it for web. If you don't have it, it will still work for the desktop versions. More info on the web version later.

To erase dependencies and builds in case you want to redo everything, run:

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
make linux   # Makes a Linux build
make windows # Makes a Windows build
make web     # Makes a WebGL (HTML5) build
make         # All of the above
```

This will create `linux.zip`, `windows.zip` and `webgl.zip` that can be sent to anyone to unzip and run the game.

To build and run a development build, run the following:

```console
bin/dev
```

It will make a build, run it and delete it after you close it. Easy for quickly compiling changes and running the game with them.

You can also make a development build that won't be ran automatically or deleted after you closing by running this:

```console
make dev
make web-dev
```

This will put the build at the project root. Can be ran on a debugger and can be sent to testers to use with the built-in dev tools. The web version builds it for web, with logging, sanitizing and assertions enabled.

When doing a dev build, you can also add a `SCENE` env var to load at the specified level, or `MUTE=1` to launch the game muted, or `NOFULLSCREEN=1` to not launch it at fullscreen, for example:

```console
SCENE=3 MUTE=1 bin/dev
```

This will run the game starting on level 3 with master volume at zero, fullscreen.

```console
NOFULLSCREEN=1 bin/dev
```

This will run the game at the start, windowed at full volume.

These `SCENE`, `NOFULLSCREEN` and `MUTE` commands only work on dev mode, that is, when running `bin/dev` or `make dev`, won't work on release builds.

To format and lint the whole project, run the following:

```console
bin/format
bin/lint
```

## Working with a web build

To make a web build, you will need to have [emsdk](https://emscripten.org/docs/getting_started/downloads.html) version 4.0.10 and `python`. Follow the link on how to install it in this version. Then, update you `.env` file with the directory you installed `emsdk` in.

If you can't compile either `raylib` or the game, pay attention to the error messages, might be some of these binaries that can't be found.

Having `raylib` installed for web, you can `make web` or `make web-dev` to make a web build on `build/dev`. Having either build there, you can serve it by running the following:

```console
bin/serve
```

This will serve the web build folder on your `localhost:3000`. The port is set on `.env`, or defaulted to `3000`. You can override it by passing a value manually, as in `PORT=5000 bin/serve`.

If you just want to quickly make a web build and run, like what happens when you do `bin/dev`, you can run:

```console
bin/web-dev
```

## License

This project is licensed under the [MIT](https://opensource.org/license/mit/) license.
