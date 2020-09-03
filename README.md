# acidcamGL

This project requires a system with a good enough graphics card to be running OpenGL 4.1 or greater. It will not work on lower specs.

To be able to download and run binaries from this project you will need to enter the terminal and type

sudo spctl --master-disable

Then go to System settings and select Security & Privacy. Select Anywhere as the location of apps.

How to compile for macOS: 

https://youtu.be/UB_i1qTLGw8

Playlist of Tutorials/Demos:

 https://www.youtube.com/playlist?list=PLUgo--lX2kmpR2VtPCj61ZWPpMMCFAu79

This project is still in very early stages of development.
This project is an implementation of Acid Cam using OpenGL/Shaders.
It is designed more for visualizations or live streaming with OBS versus a Video Editor like the other versions of Acid Cam.
The program is controlled with the qwerty keyboard but, currently looking into MIDI.


How to compile on macOS:

This project reqeuires libacidcam,GLFW,GLEW,OpenGL,OpenCV

First install homebrew: https://brew.sh

Then install GLFLW,GLEW,OpenGL,OpenCV

	brew install glfw3 glew opencv

Dependending on what you have installed this could take quite some time.

Next download libacidcam:

	git clone https://github.com/lostjared/libacidcam.git

Enter the directory

	cd libacidcam

Create a build directory

	mkdir build && cd build

Configure the project:
	
	cmake ..

Build the project

	make -j4

Install the project:

	sudo make install

Next download acidcamGL:

        git clone https://github.com/lostjared/acidcamGL.git

Enter the directory

        cd acidcamGL

Create a build directory

        mkdir build && cd build

Configure the project:

        cmake ..

Build the project

        make -j4

Run the program:

	./acidcamGL -g -p ../filters

The default Resolution is:

Camera: 1280x720
FPS: 24 
Window Size: 1280x720

you can change that using arguments passed to the program

How to use the program:
Arguments:

	-h use H.264 output w/o uses MPEG-4
	-o output mp4 filename
	-S filter start index
	-H shader start index
	-C set color map
	-T set material texture filename
	-k shortcut-key file
	-L playlist of filters
	-b restore black
	-g output debug strings
	-u fps
	-n print filter name
	-e snapshot prefix
	-p shader path
	-M monitor index
	-f fullscreen (resize)
	-F fullscreen (windowed mode)
	-d capture device
	-i input_video.mp4
	-R loop input video
	-r resolution 1920x1080
	-c Camera resolution 1280x720
	-l list filters
	-t list filters no info
	-v version

Controls:

	L - enable disable playlist
	N - set index to the end
	P - index reset to zero
	K - jump forward index by 25
	J - jump backwar index by 25
	Z - take screenshot
	F - process keyboard input for index
	S - process keyboard input for shader
	C - clear keyboard input
	[SPACE] - Acid Cam filters enabled/disabled
	[LEFT] - Filter index move left
	[RIGHT] - filter index move right
	[UP] - Shader Index move up
	[DOWN] - Shader Index move down
	[<] - Decrease index of Color Map.
	[>] - Increase index of Color Map.
	[ENTER] - Jump to currently typed index (type index with number keys at anytime)
	Q,Q,E,R,Y,U,I,O - move movement rate
	T - reset color offset

Run the program:

Full Screen Monitor 0

	./acidcamGL -g -p filters -F -M 0

or Monitor 1

	./acidcamGL -g -p filters -F -M 1

Windowed mode 1920x1080 Camera/Window

	./acidcamGL -g -p filters -c 1920x1080 -r 1920x1080

Windowed mode 1920x1080 Camera 1280x720

	./acidcamGL -g -p filters -c 1280x720 -r 1920x1080

Default Windowed  mode:

	./acidcamGL -g -p filters 

Default Fullscreen (Resize) mode:

	./acidcamGL -g -p filters -f

Default FullScreen (Windowed) mode:

	./acidcamGL -g -p filters -F

Use Webcam Device by Index:
	
	./acidcamGL -g -p filters -d 1

Use different frames per second:

	./acidcamGL -g -p filters -u 30

or (if supported by your USB 3.0 Webcam)
 
	./acidcamGL -g -p filters -u 60

or even better

	./acidcamGL -g -p filters -u 60 -c 1920x1080 -r 1920x1080

Use Video File as Input:

	./acidcamGL -g -p filters -i file.mp4

Record to MP4 file:

	./acidcamGL -g -p filters -i file.mp4 -h -o outfile.mp4

List all included filters by index:

	./acidcamGL -l

Key shortcut format:

in a text file add each line for the desired keyshort cuts in this format:

key filter_index shader_index

an example file would be

	F 75 0
	Q 25 7
If you saved this to keys.key you would use it with -k key like his:

	./acidcamGL -g -p filters -k keys.key

Playlist file:

use:
	./acidcamGL -l

Playlist Now uses String names versus ID identifiers:
to list the different filters
then list them one after the other in a text file line by line like this:

	StrobeEffect
	SelfAlphaBlend

save the file then when in the program press the L key to toggle the playlist on and off and use the arrow keys to move through the list.
To use the playlist file its he same as the keys just with
Use:

	./acidcamGL -L playist.txt -g -p filters

