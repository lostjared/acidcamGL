#generic Makefile for macOS

OBJ=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
#OPECV_LIBS= ./static/*a
CFLAGS= -F. -mmacosx-version-min=10.14 -DSYPHON_SERVER -DMIDI_ENABLED -DREACTIVE_ENABLED -std=c++11 -O2 `pkg-config glfw3 glew acidcam rtmidi rtaudio --cflags` -I./macos/include -I.
LDFLAGS=-mmacosx-version-min=10.14 `pkg-config glfw3 rtmidi rtaudio --libs` -framework VideoDecodeAcceleration  -lblas -lcblas -lclapack -llapack -lf77lapack -framework CoreVideo -framework CoreMedia -framework OpenGL -framework Cocoa -framework IOKit -framework AVKit -framework AVFoundation  -framework OpenCL  -lbz2  macos/cvlibs/*a -framework Cocoa -framework AppKit -framework Foundation

%.o: %.cpp
	$(CXX) $(CFLAGS) $< -c -o $@

acidcamGL: $(OBJ) *.hpp
	$(CXX) $(CFLAGS) -DSYPHON_SERVER syphon.mm -c
	make -C libacidcam
	$(CXX) -std=c++11 -o acidcamGL_midi  $(OBJ) syphon.o  libacidcam/libacidcam.a $(LDFLAGS) -F. -framework AGL -framework Syphon 

all: acidcamGL

.PHONY: clean
clean:
	rm -f acidcamGL_midi *.o

install:
	cp acidcamGL_midi /usr/local/bin/acidcamGL_midi
