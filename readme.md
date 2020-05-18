# Multi-Platform Triangle Template

![triangle](triangle.png)

See it running here: https://pascal-so.github.io/webgl-fun/

## Requirements

* python3 (for building glad)
* glfw3 (for native version)
* http://emscripten.org (for web version)

## Compiling

I suggest to create two build directories, one for a native build and one for the WASM build.

```bash
# Build the WASM version
mkdir buildwasm
cd buildwasm
emcmake cmake ..
make -j

# Build the native version
mkdir buildnative
cd buildnative
cmake ..
make -j
```

## Useful Resources

* https://lyceum-allotments.github.io/2016/06/emscripten-and-sdl-2-tutorial-part-1/ (doesn't really work on latest emscripten)
* https://emscripten.org/docs/porting/emscripten-runtime-environment.html#browser-main-loop
* https://book.comptechs.cn/cppwasm-by-3dgen/en/
* https://learnopengl.com/Getting-started/Hello-Triangle
