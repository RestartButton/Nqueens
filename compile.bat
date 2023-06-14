echo off
if not exist "build/bin" mkdir build\bin
g++ queen.cpp -o build/bin/queen -static