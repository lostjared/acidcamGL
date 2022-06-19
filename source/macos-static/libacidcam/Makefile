
#generic Makefile for macOS

OBJ=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
#OPECV_LIBS= ./static/*a
CFLAGS= -mmacosx-version-min=10.14 -DSYPHON_SERVER -std=c++11 -O3 `pkg-config glfw3 glew --cflags` -I. -I.. -I../../macos/include
LDFLAGS=-mmacosx-version-min=10.14 `pkg-config glfw3 --libs` -framework VideoDecodeAcceleration  -lblas -lcblas -lclapack -llapack -lf77lapack -framework CoreVideo -framework CoreMedia -framework OpenGL -framework Cocoa -framework IOKit -framework AVKit -framework AVFoundation  -framework OpenCL  -lbz2  macos/cvlibs/*a -framework Cocoa -framework AppKit -framework Foundation

%.o: %.cpp
	$(CXX) $(CFLAGS) $< -c -o $@

libacidcam.a: $(OBJ)
	$(CXX)  -mmacosx-version-min=10.14 screen.mm -c -o screen.o -I../macos/include
	ar -cru libacidcam.a *.o
all: libacidcam.a

.PHONY: clean
clean:
	rm -f  libacidcam.a  *.o

