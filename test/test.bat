@echo off
set TICK=..\build\Debug\Tick.exe

echo -----
echo Test1
echo -----
%TICK% echo.bat 1 2 3 4 0 || exit /b 1
echo -----
echo pass
echo -----
echo Test2
echo -----
%TICK% nonexisting.exe && exit /b 1
echo -----
echo pass

echo -----
echo Test3
echo -----
%TICK% echo.bat 1 2 3 4 7 && exit /b 1
if not %errorlevel% == 7 exit /b 1
echo -----
echo pass
