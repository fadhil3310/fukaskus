$ErrorActionPreference = "Stop"
Push-Location $PSScriptRoot
New-Item -ItemType Directory -Force -Path .\build
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Debug -G Ninja -DBUILD_CLI=ON
Pop-Location