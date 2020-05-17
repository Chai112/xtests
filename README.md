# xtests
Minimalist boilerplate C++ for X-Plane 11.50 plugins without an IDE\
Links against XPLM 302 SDK and OpenGL32 APIs\
Written by Chaidhat Chaimongkol in May 2020

### Usage Instructions
Instructions for Windows.\
For Mac and Linux, installation should involve homebrew.\
Please feel free to study, edit or remix the boilerplate source code.
1. Please install [MinGW-w64](https://sourceforge.net/projects/mingw-w64/), make sure it is x86_64 architecture
2. Please install [Make](http://gnuwin32.sourceforge.net/downlinks/make.php) for Windows
3. Click and run `build.bat` to compile and link the program

Violá! - find the `win.xpl` DLL plugin in the bin(aries) folder.\
\
Alternatively, if `build.bat` fails or if you are not using Windows, try using a CLI, cd int\o the directory and compile by running `make.exe`, normally installed at `C:\Program Files (x86)\GnuWin32\bin\make.exe` or with Mac/Linux, can be installed with `brew install make` and `brew install gcc` and change dependencies in the `makefile`\
\
Please find the latest X-Plane SDK [here](https://developer.x-plane.com/sdk/plugin-sdk-downloads/) and replace the  SDK  folder in `include`. succesfully built the plugin.
