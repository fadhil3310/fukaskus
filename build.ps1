$ErrorActionPreference = "Stop"

Push-Location $PSScriptRoot
Set-Location .\build
cmake --build .
Pop-Location