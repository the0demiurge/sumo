#!/bin/bash
mkdir -p build/cmake-build && cd build/cmake-build
cmake ../..
make -j$(nproc)