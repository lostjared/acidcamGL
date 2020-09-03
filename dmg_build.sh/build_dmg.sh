#!/bin/bash

mkdir -p dmg_build/acidcamGL/acidcamGL
mkdir -p dmg_build/acidcamGL/acidcamGL/filters
mkdir -p dmg_build/acidcamGL/acidcamGL/playlist
mkdir -p dmg_build/acidcamGL/acidcamGL/shortcut
mkdir -p dmg_build/acidcamGL/acidcamGL/custom
cp -rfv filters dmg_build/acidcamGL/acidcamGL/filters
cp -rfv playlist dmg_build/acidcamGL/acidcamGL/playlist
cp -rfv shortcut dmg_buildshortcut/acidcamGL/acidcamGL/shortcut
cp -rfv custom dmg_build/acidcamGL/acidcamGL/custom
cp -rfv /Applications/acidcamGL/acidcamGL.app dmg_build/acidcamGL/acidcamGL/
hdiutil create -volname acidcamGL -srcfolder dmg_build/acidcamGL -ov -format UDZO acidcamGL.dmg

