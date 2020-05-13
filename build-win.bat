@echo off
REM written by Chaidhat Chaimongkol 06-05-2020 # builds C++ to XP11 WINDOWS
REM
REM download MinGW w64-bit AND install with x86_64 architecture
REM https://sourceforge.net/projects/mingw-w64/

REM make directory to put binaries in
MKDIR bin

REM GCC compile & assemble
gcc ^
main.cpp ^
-o bin/main.o ^
-m64 ^
-c ^
-DIBM ^
-DXPLM200 ^
-DXPLM210 ^
-DXPLM300 ^
-DXPLM301 ^
-DXPLM302 ^
-Iinc/SDK/XPLM ^
-Iinc/SDK/Widgets 


REM GCC link
gcc ^
bin/main.o ^
-o bin/win.xpl ^
-shared ^
-Llib/SDK/Win ^
-lopengl32 ^
-lXPLM_64 ^
-lXPWidgets_64

ECHO Installing...

COPY bin\win.xpl "D:\Documents\programs\X-Plane 11\Aircraft\Chai112\b777-300er\plugins"
COPY bin\win.xpl "D:\Documents\programs\X-Plane 11\Aircraft\Chai112\xtest1\plugins"
COPY bin\win.xpl "D:\Documents\programs\X-Plane 11\Aircraft\Chai112\xtest2\plugins"

ECHO Finished.
PAUSE
