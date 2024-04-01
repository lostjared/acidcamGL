# acidcamGL

![ScreenShot1](https://github.com/lostjared/acidcamGL/blob/master/screens/obs.jpg?raw=true "screenshot1")

![ScreenShot2](https://github.com/lostjared/acidcamGL/blob/master/screens/screen_linux.jpg?raw=true "screenshot2")

![ScreenShot3](https://github.com/lostjared/acidcamGL/blob/master/screens/windows.jpg?raw=true "screenshot3")

Join us on Discord: https://discord.gg/kSxTe6M8



This project requires a system with a good enough graphics card to be running OpenGL 3.3 or greater. It will not work on lower specs.

To be able to download and run binaries from this project you will need to enter the terminal and type

sudo spctl --master-disable

Then go to System settings and select Security & Privacy. Select Anywhere as the location of apps.

How to compile for macOS: 

This project is still in very early stages of development.
This project is an implementation of Acid Cam using OpenGL/Shaders.
It is designed more for visualizations or live streaming with OBS versus a Video Editor like the other versions of Acid Cam.
The program is controlled with the qwerty keyboard but, currently looking into MIDI.
The best way to use this program is to compile an up to date version from source code.

How to compile on macOS:

This project reqeuires libacidcam,GLFW,GLEW,OpenGL,OpenCV,FFmpeg

First install homebrew: https://brew.sh

Then install GLFW,GLEW,OpenGL,OpenCV,FFmpeg

	brew install glfw3 glew opencv ffmpeg

Dependending on what you have installed this could take quite some time.
You can choose to statically compile

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
Install it

        sudo make install

for macOS static build with Syphon enter the macos-static directory and

	make -j4
    
You will still need to use install_name_tool on an app bundle see copy.sh for how I do it

for Linux build with Desktop capture support (only Xorg not Wayland) enter source directory and

	make -f Makefile.linux && sudo make -f Makefile.linux install

Run the program: (recommd first exporting shell variable SHADER_PATH with path to filters) example:

    export SHADER_PATH=/home/jared/acidcamGL/filters
    
or pass the path when executing the program

    acidcamGL -g -p ../filters
    
The source/plugin directory contains some test files I have been working on.

The default Resolution is:

Camera: 1280x720
FPS: 24 
Window Size: 1280x720

you can change that using arguments passed to the program
How to use the program:

Arguments:
- `-A <autofilter file>`: Specify an autofilter file to use.
- `-X <codec>`: Set the codec for video encoding.
- `-o <output filename>`: Define the output filename for the video.
- `-S <filter start index>`: Set the starting index for filters.
- `-H <shader start index>`: Specify the starting index for shaders.
- `-Z <Starting filter by Name>`: Specify the starting filter by its name.
- `-C <set color map>`: Set the color map.
- `-T <set material texture filename>`: Specify the material texture filename.
- `-s <image output format>`: Set the image output format (e.g., png, jpg).
- `-N <play list slideshow timeout>`: Set the playlist slideshow timeout.
- `-k <shortcut-key file>`: Specify the shortcut-key file.
- `-L <playlist of filters>`: Specify a playlist of filters.
- `-b`: Restore black in the output.
- `-g`: Output debug strings.
- `-u <fps>`: Set the frames per second for the video.
- `-n`: Print the name of the currently applied filter.
- `-e <snapshot prefix>`: Set the prefix for snapshot filenames.
- `-p <shader path>`: Specify the path to the shader files.
- `-M <monitor index>`: Select the monitor index for fullscreen mode.
- `-f`: Enable fullscreen (resize to fit).
- `-F`: Enable fullscreen (windowed mode).
- `-d <capture device>`: Specify the capture device.
- `-i <input_video.mp4>`: Specify the input video file.
- `-R`: Loop input video.
- `-x <Stereo Mode>`: Enable stereo mode.
- `-r <resolution 1920x1080>`: Set the resolution.
- `-c <Camera resolution 1280x720>`: Set the camera resolution.
- `-G`: Enable screen capture mode.
- `-Y`: Enable Syphon Server (macOS only).
- `-U <Screen Capture Position X,Y>`: Set the screen capture position.
- `-P`: Redirect standard output to socket.
- `-W <custom filter path>`: Specify a custom filter path.
- `-B`: Enable playback filter mode.
- `-q`: Shuffle playlist.
- `-w <beats per minute for shuffle>`: Set beats per minute for shuffle mode.
- `-l`: List filters.
- `-t`: List filters without additional information.
- `-v`: Display the version information.
- `-4`: Enable FFmpeg x264 support.
- `-5`: Enable FFmpeg x265 support.
- `-m <crf for x265 for video mode>`: Specify the CRF for x265 encoding mode.
- `-1 <ffmpeg path>`: Set the path to the FFmpeg executable.
- `-7 <video start offset in seconds>`: Set the video start offset in seconds.
- `-3 <Plugin Path>`: Specify the plugin path.
- `-9 <Number of threads>`: Set the number of threads for processing.
- `-8 <Set Max Allocated Frames>`: Set the maximum number of allocated frames.
- `--mux <outputted_file source_file>`: Mux audio (copy audio) from the source file to the outputted file.
- `--list`: Display the list of available capture devices.
- `--rotate <src output>`: Rotate the source video and output.
Enviorment Variables: (you should export these)

        SHADER_PATH - path to shaders	
        AC_PLUGIN_PATH - path to compiled plugins
        AC_CUSTOM_PATH - path to custom filter stack text files
        FFMPEG_PATH - path to ffmpeg executable
Controls:
    
    L - enable disable playlist
    N - set index to the end
    P - index reset to zero
    K - jump forward index by 25
    J - jump backwar index by 25
    Z - take screenshot
    H - Shuffle Playlist
    [SHIFT]+H - AutoFilter Shuffle Lock On/Off
    M - Enable/Disable Playlist Slideside random timeout
    F - process keyboard input for index
    S - process keyboard input for shader
    C - clear keyboard input
    [+] - increase blend percentage
    [-] = decrease blend percentage
    [SPACE] - Acid Cam filters enabled/disabled
    [SHIFT]+[SPACE] - Toggle Autofilter on and off
    [LEFT] - Filter index move left
    [RIGHT] - filter index move right
    [UP] - Shader Index move up
    [DOWN] - Shader Index move down
    [ENTER] - Jump to currently typed index (type index with number keys at anytime)
    [SHIFT]+[ENTER] - Release Stored Frames
    [PAGEUP] - Store Index Position
    [PAGEDOWN] - Restore Position Index
     Q,Q,E,R,Y,U,I,O - move movement rate
     T - reset color offset


Run the program:
You will need to ethier set the SHADER_PATH enviorment variable or pass the path to the shaders when exeucting the program with the -p argument. Each shader is listed in a index.txt file in the order you want the program to cycle through them. The program comes with over 100 example shaders.
Example exporting the enviorment varaible:

      export SHADER_PATH=/Users/jared/Downloads/acidcamGL/filters


Launch Webcam Device:

	acidcamGL -d 0 -g -p filters

Launch Filepath

	acidcamGL -i file_path -g -p filters

If you device has smaller amount of ram set the memory pool size in frames Examples would be:
	
	acidcamGL -i file_path -g -8 100

or
	
	acidcamGL -i file_path -g -8 500


Full Screen Monitor 0

	acidcamGL -g -p filters -F -M 0

or Monitor 1

	acidcamGL -g -p filters -F -M 1

Windowed mode 1920x1080 Camera/Window

	acidcamGL -g -p filters -c 1920x1080 -r 1920x1080

Windowed mode 1920x1080 Camera 1280x720

	acidcamGL -g -p filters -c 1280x720 -r 1920x1080

Default Windowed  mode:

	acidcamGL -g -p filters 

Default Fullscreen (Resize) mode:

	acidcamGL -g -p filters -f

Default FullScreen (Windowed) mode:

	acidcamGL -g -p filters -F

Use Webcam Device by Index:
	
	acidcamGL -g -p filters -d 1

Use different frames per second:

	acidcamGL -g -p filters -u 30

or (if supported by your USB 3.0 Webcam)
 
	acidcamGL -g -p filters -u 60

or even better

	acidcamGL -g -p filters -u 60 -c 1920x1080 -r 1920x1080

Use Video File as Input:

	acidcamGL -g -p filters -i file.mp4

Record to MP4 file:

	acidcamGL -g -p filters -i file.mp4 -h -o outfile.mp4

Record as x264 using FFmpeg:
	
	acidcamGL -g -p filters -i file.mp4 -4 -o output.mp4 

Record as x265 using FFmpeg

	acidcamGL -g -p filters -i file.mp4 -5 -o output.mp4

List all included filters by index:

	acidcamGL -l

Jump to location in vidoe file by seconds

	acidcamGL -g -p filters -i file.mp4 -7 25


Key shortcut format:

in a text file add each line for the desired keyshort cuts in this format:

key filter_index shader_index

an example file would be

	F 75 0
	Q 25 7
If you saved this to keys.key you would use it with -k key like his:

	acidcamGL -g -p filters -k keys.key

Playlist file:

use:
	acidcamGL -l

Playlist Now uses String names versus ID identifiers:
to list the different filters
then list them one after the other in a text file line by line like this:

	StrobeEffect
	SelfAlphaBlend

save the file then when in the program press the L key to toggle the playlist on and off and use the arrow keys to move through the list.
To use the playlist file its he same as the keys just with
Use:

	acidcamGL -L playist.txt -g -p filters

After enabling plugins (used for testing) (with the shell variable pointing to path) and having the list of plugins in index.txt to load one from the command line use:

	acidcamGL -g -p filters -Z plugin.acidcam

Project comes with some example test plugin files in the acidcamGL/source/plugin folder. To compile these enter the directory and type 

    make

This will install the examples in this tree under acidcamGL/plugin
To export these after compiling them use

    export AC_PLUGIN_PATH=/path/to/acidcamGL/plugin
    
add the export commands to your .bash_profile to not have to issue them everytime you start a new session  in the terminal.

To use stereo mode, have two webcams of the same type and use:

    acidcamGL -d camera1_index -6 camera2_index -g -p ./filters -c 1280x720 -r 1280x720 -u 30
    
This will use camera1-index as left eye camera 2 index as right eye  as resolution 1280x720 30 fps

You can also turn a regular video into stereo mode by using the -x argument

    acidcamGL -i file.mp4 -g -x

Or press the '1' key to toggle stereo on and off.

Press < and > to shuffle through different Color Maps.
