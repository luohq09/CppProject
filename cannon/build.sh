#!/usr/bin sh

rm -rf build
mkdir build
cd build

source /opt/rh/devtoolset-3/enable || true
cmake ..
make clean
make -j8

# prepare test
#cp ./src/cannon/test/bin/storage_test ./bin
mkdir release
mkdir release/logs
mv bin release/
cp -r ../env/* ./
mkdir test/ut_logs
mkdir test/perf_logs
cp -r ../scripts/* release/bin/
