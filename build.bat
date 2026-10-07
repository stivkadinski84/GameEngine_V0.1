@echo off
echo Building Game...
gcc src/core/*.c src/graphics/*.c src/platform/windows/*.c src/input/*.c src/audio/*.c examples/basic_2d/*.c -Iinclude -o game.exe -lwinmm -lgdi32 -luser32
if %errorlevel% neq 0 (
    echo Build Failed!
    pause
    exit /b 1
)
echo Build Successful
pause