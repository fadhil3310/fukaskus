$ErrorActionPreference = "Stop"

Push-Location $PSScriptRoot
cmake --build build
Pop-Location