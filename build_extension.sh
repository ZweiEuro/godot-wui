#!/bin/bash
# configure
cmake -DBUILD_EXTENSION_STNADALONE=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_INSTALL_PREFIX=/home/zweieuro/Documents/uni/2023_SS/WUI/core/godot-wui/.out/install/gcc-debug -S/home/zweieuro/Documents/uni/2023_SS/WUI/core/godot-wui -B/home/zweieuro/Documents/uni/2023_SS/WUI/core/godot-wui/.out/build/gcc-debug -G Ninja
# build
cmake --build ./build/gcc-debug --parallel 18 --