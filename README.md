# uxx

Simple immediate mode GUI library for C++20.

|Compiler|Status|
|--------|------|
|MSVC 19.26|[![AppVeyor Build status](https://img.shields.io/appveyor/ci/kjetand/cpp-wolf.svg)](https://ci.appveyor.com/project/kjetand/cpp-wolf)|
|GCC 10.1|[![Build Status](https://travis-ci.org/kjetand/uxx.svg?branch=master)](https://travis-ci.org/kjetand/uxx)|

## Build

### Requirements

| OS       | Build tool      | Compiler        |
|----------|-----------------|-----------------|
| Linux    | `cmake >= 3.15` | `g++ >= 10.1`   |
| Windows  | `cmake >= 3.15` | `msvc >= 19.26` |

### Dependencies

- `OpenGL`
- `SFML >= 2.5.1` (automatically downloaded by `cmake`)

### Instructions

```text
$ cd uxx/
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

The above steps should apply to both Windows and Linux. However, on windows you may need to specify `cmake.exe` and
run with explicit generator `$ cmake.exe -G "Visual Studio 16 2019" ..` instead of just `$ cmake.exe ..`.

## License

Licensed under the [MIT License](LICENSE).

Special thanks to:

- Omar Cornut and ImGui contributors (https://github.com/ocornut/imgui)
- Elias Daler (https://github.com/eliasdaler/imgui-sfml)
- Jonathan Müller (https://github.com/foonathan/type_safe)
- Catch2 team (https://github.com/catchorg/Catch2)
