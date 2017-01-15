LIBDIRS= -L/usr/X11R6/lib
LDLIBS = -lglut -lGL -lGLU -lX11 -lm -lpng

#Extra flags to give to the C PreProcessor, specify #include directories
CPPFLAGS=# -O3

#Extra flags to give to compilers when they are supposed to invoke the linker, ‘ld’, such as -L.
#Libraries (-lfoo) should be added to the LDLIBS variableinstead.
LDFLAGS = $(CPPFLAGS) $(LIBDIRS)

TARGETS = maze game_object

SRCS = *.cpp

OBJS =  $(SRCS:.cpp=.o)

#-Wall	include all compiler warnings
#
#
CXX = g++ -std=c++11 -o -c #-Wall

$(TARGETS) : $(SRCS)
