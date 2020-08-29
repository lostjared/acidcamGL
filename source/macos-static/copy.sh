#!/bin/sh

make clean && make -j16 && cp ./acidcamGL /Applications/acidcamGL/acidcamGL.app/Contents/MacOS/acidcamGL && open /Applications/acidcamGL/acidcamGL.app --args -g -p /Applications/acidcamGL/filters
