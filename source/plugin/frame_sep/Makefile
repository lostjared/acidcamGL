
#generic Makefile for macOS

PROGRAM=frame_sep.acidcam
OBJ=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
#OPECV_LIBS= ./static/*a
#CFLAGS= -F. -mmacosx-version-min=10.13 -DSYPHON_SERVER -O3 -std=c++11 -O2 `pkg-config glfw3 glew acidcam` -I../../macos/include -I../..
#LDFLAGS=-mmacosx-version-min=10.13 `pkg-config glfw3 --libs` -framework VideoDecodeAcceleration  -lblas -lcblas -lclapack -llapack -lf77lapack -framework CoreVideo -framework CoreMedia -framework OpenGL -framework Cocoa -framework IOKit -framework AVKit -framework AVFoundation  -framework OpenCL  -lbz2  ../../macos/cvlibs/*a -framework Cocoa -framework AppKit -framework Foundation
#LDFLAGS=`pkg-config glfw3 glew opencv4 --libs`
CFLAGS=-O3 -std=c++11 `pkg-config acidcam opencv4 --cflags` -fPIC
LDFLAGS=`pkg-config opencv4  acidcam --libs`
%.o: %.cpp
	$(CXX) $(CFLAGS) $< -c -o $@

$(PROGRAM): $(OBJ) 
	$(CXX) -O3 -std=c++11 -shared -o $(PROGRAM) $(OBJ) $(LDFLAGS)
#../../libacidcam/libacidcam.a

all: $(PROGRAM)

.PHONY: clean
clean:
	rm -f $(PROGRAM)  *.o

install: all
	cp $(PROGRAM)  ../../../plugin
#/Users/jared/Source/newestac2/acidcamGL/plugin

