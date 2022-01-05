#!/bin/sh

echo " build project "
mkdir acidcam
cd acidcam
git clone https://github.com/lostjared/libacidcam.git
cd libacidcam
mkdir build
cd build
cmake ..
make -j8
sudo make install
cd ../../
git clone https://github.com/lostjared/acidcamGL.git
cd acidcamGL
mkdir build
cd build
cmake ..
make -j8
sudo make install
cd ../../
echo "success"

