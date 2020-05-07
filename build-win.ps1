# written by Chaidhat Chaimongkol 06-05-2020
# builds c++ to XP11 WINDOWS
#
# Download MinGW 64-bit AND install with x86_64 architecture
# https://sourceforge.net/projects/mingw-w64/

$compiler = 'g++'
$src = 'main.cpp'
$target = '-o win.xpl'
$flags = '-m64'         # 64 bit only

$lib = @"
-Lsdk/libraries/win
-lopengl32
-lXPLM_64
-lXPWidgets_64
"@

$inc = @"
-ID:\Documents\code\repo\b\xtests\SDK\CHeaders\XPLM
-Isdk/cheaders/Widgets
"@

$def = @"
-DIBM     `
-DXPLM200 `
-DXPLM210 `
-DXPLM300
-DXPLM301
-DXPLM302
"@

# set working directory
$scriptpath = $MyInvocation.MyCommand.Path
$dir = Split-Path $scriptpath
cd $dir
ls

# compile
# & $compiler  $flags $inc $lib $def $target $src  

gcc -ID:\Documents\code\repo\b\xtests\SDK\CHeaders\XPLM -Isdk/cheaders/Widgetsr -DIBM -DXPLM200 -DXPLM210 -DXPLM300 -DXPLM301 -DXPLM302 -o win.o main.cpp -c -shared

&gcc
 win.o
 -Lsdk/libraries/win
 -lopengl32
 -lxplm_64
 -lwpwidgets_64
 -shared
 -o win.xpl
pause
