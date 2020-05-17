# Multi-Platform SDL2 template

Adapted from https://github.com/timhutton/opengl-canvas-wasm

## Requirements

* SDL2
* http://emscripten.org

## Compiling

I suggest to create two build directories, one for a native build and one for the WASM build.

```bash
# Build the WASM version
mkdir buildwasm
cd buildwasm
emcmake cmake -DCMAKE_BUILD_TYPE=Release ..
make -j

# Build the native version
mkdir buildnative
cd buildnative
cmake ..
make -j
```

## Known Issues

* The WASM version currently doesn't work with Debug mode. `-s ASSERTIONS=1` complains about `emscripten_set_main_loop` and `-s SAFE_HEAP=1` complains that "The application has corrupted its heap memory area (address zero)!". If you know how to fix this then please tell me, from what it looks like on the internet everyone just seems to go along withit and switches to release mode, it does work just fine after all...

* Responsiveness to resizing doesn't work yet.

* Native build segfaults ¯\\_(ツ)_/¯

## Useful Resources

* https://lyceum-allotments.github.io/2016/06/emscripten-and-sdl-2-tutorial-part-1/ (doesn't really work on latest emscripten)
* https://emscripten.org/docs/porting/emscripten-runtime-environment.html#browser-main-loop
* https://book.comptechs.cn/cppwasm-by-3dgen/en/
