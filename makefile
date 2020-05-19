# https://stackoverflow.com/questions/40621451/makefile-automatically-compile-all-c-files-keeping-o-files-in-separate-folde
# written by Chaidhat Chaimongkol 15-05-2020 
# builds C++ for XP11 Windows

CC=gcc # use GCC as a compiler


# Directories
BDIR = bin# dir for binaries
ODIR := obj# dir for C++ objects
SDIR := src
IDIR = include/sdk/cheaders# dir for headers
LDIR = include/sdk/libraries/win# dir for libs

SRC := $(wildcard $(SDIR)/*.cpp)    # all C++ files

# Objects
_OBJ := $(SRC:.cpp=.o) # all C++, except suffix is .o
OBJ = $(patsubst $(SDIR)/%,$(ODIR)/%,$(_OBJ)) # append ODIR

# Dependencies
_DEPS = xplm widgets
DEPS = $(addprefix -I$(IDIR)/,$(_DEPS)) # append -I and IDIR

# Libraries
_LIBS = XPLM_64 XPWidgets_64 opengl32 
LIBS = $(addprefix -l, $(_LIBS)) # append -l

# Definitions
_DEFS = IBM XPLM200 XPLM210 XPLM300 XPLM301 XPLM302							
DEFS = $(addprefix -D, $(_DEFS)) # append -D

# Flags
CXXFLAGS = -m64 $(DEFS) $(DEPS) 
LDFLAGS = -shared -L$(LDIR) $(LIBS) 

%.cpp:
	echo $@

$(ODIR)/%.o: $(SDIR)/%.cpp $(SDIR)/%.hpp
	$(CC) -c -o $@ $< $(CXXFLAGS)

$(BDIR)/win.xpl: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) 
	$(MAKE) clean
