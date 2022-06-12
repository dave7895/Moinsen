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
- [doctest](https://github.com/doctest/doctest) - The fastest feature-rich C++11/14/17/20 single-header testing framework
- [libchess](https://github.com/kz04px/libchess) - "A C++17 library that should provide legal move generation and be fairly performant."