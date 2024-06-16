#!/bin/sh

make clean && make -j16 && cp ./acidcamGL /Applications/acidcamGL/acidcamGL.app/Contents/MacOS/acidcamGL 
install_name_tool -change @loader_path/../Frameworks/Syphon.framework/Versions/A/Syphon  @executable_path/../Frameworks/Syphon.framework/Versions/A/Syphon  /Applications/acidcamGL/acidcamGL.app/Contents/MacOS/acidcamGL 
cp acidcamGL_midi /usr/local/bin
