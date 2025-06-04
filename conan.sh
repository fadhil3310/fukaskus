#!/usr/bin/env bash
set -e
cd "$(dirname "$0")"

if test -f ./conanprofile_linux.txt; then
    conan install . -pr ./conanprofile_linux.txt --output-folder=build --build=missing -s build_type=Debug
else
    conan install . --output-folder=build --build=missing -s build_type=Debug
fi