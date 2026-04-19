@echo off
pushd %~dp0\..\
call MyLib\premake\bin\premake5.exe vs2022
popd
PAUSE