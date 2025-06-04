#!/usr/bin/env bash
set -e
cd "$(dirname "$0")"

mkdir -p build
cmake -B build -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Debug -G Ninja -DBUILD_CLI=OFF