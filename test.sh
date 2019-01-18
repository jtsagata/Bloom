#!/bin/bash

killall bshell
clang-format-7 -i src/*.cpp src/*.h

pushd cmake-build-debug
make && ./src/bshell $@
