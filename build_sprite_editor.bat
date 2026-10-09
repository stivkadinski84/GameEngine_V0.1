@echo off
echo Building Sprite Editor...

gcc src/core/*.c ^
    src/graphics/*.c ^
    src/platform/windows/*.c ^
    src/audio/*.c ^
    src/input/*.c ^
    src/utils/2D/*.c ^
    tools/ui/platform/windows/utils/side_panel.c ^
    tools/ui/platform/windows/utils/buttons.c ^
    tools/sprite_editor/sprite_editor.c ^
    -Iinclude ^
    -I. ^
    -Itools ^
    -Itools/sprite_editor ^
    -o sprite_editor.exe ^
    -lwinmm -lgdi32 -luser32

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful!
) else (
    echo.
    echo Build failed!
)

pause