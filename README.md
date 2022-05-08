# Moinsen
A C++20 UCI chess engine

## Build instructions
```bash
git clone --recurse-submodules https://github.com/dave7895/Moinsen
mkdir Moinsen/libchess/build && cd Moinsen/libchess/build
cmake .. && cmake --build .
mkdir ../../build && cd ../../build
cmake .. && cmake --build .
```

## Dependencies
- [Catch2](https://github.com/catchorg/Catch2) - "A modern, C++-native, test framework for unit-tests, TDD and BDD - using C++14, C++17 and later (C++11 support is in v2.x branch, and C++03 on the Catch1.x branch)"
- [libchess](https://github.com/kz04px/libchess) - "A C++17 library that should provide legal move generation and be fairly performant."