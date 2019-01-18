#!/bin/bash

echo "Generate documentation ..."

clang-format-7 -i src/*.cpp *.h

doxygen Doxyfile
pushd doc/latex

make
cp refman.pdf ../../BloomReport.pdf

popd

cloc src/* CMakeLists.txt examples/*
