$ErrorActionPreference = "Stop"

Push-Location $PSScriptRoot
cmake --install build --prefix ./install
Pop-Location