
#generic Makefile for macOS

OBJ=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
#OPECV_LIBS= ./static/*a
CFLAGS= -F. -mmacosx-version-min=10.13 -DSYPHON_SERVER -std=c++11 -O2 `pkg-config glfw3 glew acidcam` -I../../macos/include -I.
LDFLAGS=-mmacosx-version-min=10.13 `pkg-config glfw3 --libs` -framework VideoDecodeAcceleration  -lblas -lcblas -lclapack -llapack -lf77lapack -framework CoreVideo -framework CoreMedia -framework OpenGL -framework Cocoa -framework IOKit -framework AVKit -framework AVFoundation  -framework OpenCL  -lbz2  ../../macos/cvlibs/*a -framework Cocoa -framework AppKit -framework Foundation

%.o: %.cpp
	$(CXX) $(CFLAGS) $< -c -o $@

plug1: $(OBJ) 
	$(CXX) -std=c++11 -shared -o plug1.acidcam $(OBJ) $(LDFLAGS)

all: plug1.acidcam

.PHONY: clean
clean:
	rm -f plug1.acidcam  *.o

