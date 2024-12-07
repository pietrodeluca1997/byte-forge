@echo off
setlocal EnableDelayedExpansion

set OUTPUT_PATH=..\..\..\Build
set OBJECT_PATH=%OUTPUT_PATH%\intermediate
set EXECUTABLE_NAME=%OUTPUT_PATH%\TopDownShooter.exe
set INCLUDE_FLAGS=-I..\include -I..\source
set LINKER_FLAGS=
set COMPILER_FLAGS=-g -Wall
set OBJECT_FILES=

if not exist %OUTPUT_PATH% mkdir %OUTPUT_PATH%
if not exist %OBJECT_PATH% mkdir %OBJECT_PATH%

for /r ..\ %%f in (*.c) do (
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