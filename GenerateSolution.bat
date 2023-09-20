@echo off
echo Ensuring 'build' folder exists...
mkdir .\build
echo .

cd build
cmake.exe -G "Visual Studio 17 2022" .\..
cd ..
powershell .\FixupProjectSettings.ps1
pause