# written by Chaidhat Chaimongkol 06-05-2020
# builds c++ to XP11 WINDOWS

$compiler = 'gcc'
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
-Isdk/cheaders/XPLM
-Isdk/cheaders/Widgets
"@

$def = @"
-DIBM
-DXPLM200
-DXPLM210
-DXPLM300
-DXPLM301
-DXPLM302
"@

# set working directory
$scriptpath = $MyInvocation.MyCommand.Path
$dir = Split-Path $scriptpath
cd $dir

# compile
& $compiler $target $src $flags $lib $inc $def
pause
