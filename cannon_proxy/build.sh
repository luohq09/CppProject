#!/usr/bin sh

rm -rf build
mkdir build
cd build

cmake ..
make clean
make -j8
