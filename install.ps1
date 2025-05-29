$ErrorActionPreference = "Stop"

Push-Location $PSScriptRoot
if (Test-Path -Path "./conanprofile.txt")
{
    conan install . -pr ./conanprofile.txt --output-folder=build --build=missing -s build_type=Debug
} else {
    conan install . --output-folder=build --build=missing -s build_type=Debug
}
Pop-Location