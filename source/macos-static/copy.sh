#!/bin/sh

make clean && make -j16 && cp ./acidcamGL /Applications/acidcamGL/acidcamGL.test.app/Contents/MacOS/acidcamGL && open /Applications/acidcamGL/acidcamGL.test.app --args -g -p /Applications/acidcamGL/filters
