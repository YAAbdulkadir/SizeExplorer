@echo off
echo Compiling SizeExplorer...

:: Ensure bin directory exists
if not exist bin mkdir bin

:: Compile with g++
g++ -std=c++17 -o bin\SizeExplorer.exe src\SizeExplorer.cpp

:: Check if compilation was successful
if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed!
    exit /b %ERRORLEVEL%
)

echo Compilation successful! Executable is located in bin\SizeExplorer.exe