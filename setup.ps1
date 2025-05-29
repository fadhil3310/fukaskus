$ErrorActionPreference = "Stop"
Push-Location $PSScriptRoot
New-Item -ItemType Directory -Force -Path .\build
Set-Location .\build
cmake .. -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Debug -G Ninja
Pop-Location