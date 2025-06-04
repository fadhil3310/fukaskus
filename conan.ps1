$ErrorActionPreference = "Stop"

Push-Location $PSScriptRoot
if (Test-Path -Path "./conanprofile_win.txt")
{
    conan install . -pr ./conanprofile_win.txt --output-folder=build --build=missing -s build_type=Debug
} else {
    conan install . --output-folder=build --build=missing -s build_type=Debug
}
Pop-Location