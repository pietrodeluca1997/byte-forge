@echo off
setlocal EnableDelayedExpansion

set OUTPUT_PATH=..\..\..\Build
set OBJECT_PATH=%OUTPUT_PATH%\intermediate
set EXECUTABLE_NAME=%OUTPUT_PATH%\Pong.exe
set INCLUDE_FLAGS=-I..\include -I..\source -I..\..\..\Libraries\SDL2\include -I..\..\..\Modules\Platform\include -I..\..\..\Modules\ECS\include
set LINKER_FLAGS=-L..\..\..\Libraries\SDL2\libraries -lSDL2.lib -lSDL2main.lib
set COMPILER_FLAGS=-g -Wall
set OBJECT_FILES=

if not exist %OUTPUT_PATH% mkdir %OUTPUT_PATH%
if not exist %OBJECT_PATH% mkdir %OBJECT_PATH%

for /r ..\..\..\ %%f in (*.c) do (
    set OBJECT_FILE=%OBJECT_PATH%\%%~nf.o

    if not exist !OBJECT_FILE! (
        clang -c %%f -o !OBJECT_FILE! %INCLUDE_FLAGS% %COMPILER_FLAGS%
    ) else if %%f GTR !OBJECT_FILE! (
        clang -c %%f -o !OBJECT_FILE! %INCLUDE_FLAGS% %COMPILER_FLAGS%
    )

    set OBJECT_FILES=!OBJECT_FILES! !OBJECT_FILE!
)

clang %COMPILER_FLAGS% !OBJECT_FILES! %INCLUDE_FLAGS% %LINKER_FLAGS% -o %EXECUTABLE_NAME%

endlocal