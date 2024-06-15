# acidcamGL

![ScreenShot1](https://github.com/lostjared/acidcamGL/blob/master/screens/obs.jpg?raw=true "screenshot1")

![ScreenShot2](https://github.com/lostjared/acidcamGL/blob/master/screens/screen_linux.jpg?raw=true "screenshot2")

![ScreenShot3](https://github.com/lostjared/acidcamGL/blob/master/screens/windows.jpg?raw=true "screenshot3")

# Motivation

Just to create something that I can use to create fun video and artwork to share online.

# Notes:

If downloading the Windows setup be sure to update to the most up-to-date shaders by downloading the project source code
and copying the files in the filter/ directory into the same directory in the installation folder.
Otherwise, you will be using an outdated list of default filters. The default folder for installation is:


C:\Program Files\LostSideDead\acidcamGL\acidcamGL\acidcamGL_x64

so the shaders directory is:

C:\Program Files\LostSideDead\acidcamGL\acidcamGL\acidcamGL_x64\filters


# Requirements

Update: To compile in MIDI support (this is still in the progress of development) install rtmidi on Debian its called:

 librtmidi-dev

```bash
make -f Makefile.midi.linux
sudo make -f Makefile.midi.linux install
```
or on macOS use
```bash
make -f Makefile.midi
sudo make -f Makefile.midi install
```

To configure your MIDI controller, follow these steps:

- Compile and run the midi-cfg configuration tool in source code sub-directory midi-cfg/

- Use the tool to configure your MIDI controller.

- Copy the outputted configuration file, midi.midi_cfg, to your application directory.

- Set the environment variable by running the following command to point to the location of midi.midi_cfg

```bash
export AC_MIDI=/Users/jared/acidcamGL/midi.midi_cfg
```

- Run the application like normal

Your MIDI controller should now be configured and ready for use with the program.
Be sure to run the right acidcamGL program the Linux and MacOS Makefiles give the program different names

## Requirements

This project requires a system with a good enough graphics card to be running OpenGL 3.3 or greater. It will not work on lower specs.
Please read this entire document before using the software. On MacOS you may have to give the Terminal or the application privileges to use the webcam and capture the desktop.
This program requires a lot of ram. If you wish to run with lower ram set 
the size of the memory pool with -8 flag. Example:

```bash
	acidcamGL -d 0 -c 1920x1080 -r 1920x1080 -u 30 -8 200
```
# How to compile on macOS:

This project requires libacidcam,GLFW,GLEW,OpenGL,OpenCV,FFmpeg

First, install homebrew: https://brew.sh

Then install GLFW,GLEW,OpenGL,OpenCV,FFmpeg

	brew install glfw3 glew opencv ffmpeg

Depending on what you have installed this could take quite some time.
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

# How to Compile on Linux

First install the required libraries

for Linux build with Desktop capture support (only Xorg not Wayland) enter source directory and

	make -f Makefile.linux && sudo make -f Makefile.linux install

Otherwise follow the same instructions as macOS, clone libacidcam, compile it, install it, clone acidcamGL, compile it, install it.

# Running the program

Run the program: (recommend first exporting shell variable SHADER_PATH with path to filters) example:

    export SHADER_PATH=/home/jared/acidcamGL/filters
    
or pass the path when executing the program

    acidcamGL -g -p ../filters
    
The source/plugin directory contains some test files I have been working on.

# Defaults

The default Resolution is:

Camera: 1280x720
FPS: 24 
Window Size: 1280x720

you can change that using arguments passed to the program
How to use the program:

# Arguments

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

# Environment Variables

- `SHADER_PATH`: Path to shaders.
- `AC_PLUGIN_PATH`: Path to compiled plugins.
- `AC_CUSTOM_PATH`: Path to custom filter stack text files.
- `FFMPEG_PATH`: Path to ffmpeg executable.Controls:

# Keyboard Input

- `L`: Enable/disable playlist.
- `N`: Set index to the end.
- `P`: Index reset to zero.
- `K`: Jump forward index by 25.
- `J`: Jump backward index by 25.
- `Z`: Take a screenshot.
- `H`: Shuffle Playlist.
- `[SHIFT]+H`: AutoFilter Shuffle Lock On/Off.
- `M`: Enable/Disable Playlist Slideshow random timeout.
- `F`: Process keyboard input for index.
- `S`: Process keyboard input for shader.
- `C`: Clear keyboard input.
- `[+]`: Increase blend percentage.
- `[-]`: Decrease blend percentage.
- `[SPACE]`: Acid Cam filters enabled/disabled.
- `[SHIFT]+[SPACE]`: Toggle AutoFilter on and off.
- `[LEFT]`: Filter index move left.
- `[RIGHT]`: Filter index move right.
- `[UP]`: Shader Index move up.
- `[DOWN]`: Shader Index move down.
- `[ENTER]`: Jump to currently typed index (type index with number keys at any time).
- `[SHIFT]+[ENTER]`: Release Stored Frames.
- `[PAGEUP]`: Store Index Position.
- `[PAGEDOWN]`: Restore Position Index.
- `Q, Q, E, R, Y, U, I, O`: Move movement rate.
- `T`: Reset color offset.


Run the program:
You will need to either set the SHADER_PATH environment variable or pass the path to the shaders when executing the program with the -p argument. Each shader is listed in an index.txt file in the order you want the program to cycle through them. The program comes with over 100 example shaders.
Example exporting the environment variable:

      export SHADER_PATH=/Users/jared/Downloads/acidcamGL/filters


Launch Webcam Device:

	acidcamGL -d 0 -g -p filters

Launch Filepath

	acidcamGL -i file_path -g -p filters

If your device has a smaller amount of RAM set the memory pool size in frames Examples would be:
	
	acidcamGL -i file_path -g -8 100

or
	
	acidcamGL -i file_path -g -8 500


Full-Screen Monitor 0

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

Jump to location in video file by seconds

	acidcamGL -g -p filters -i file.mp4 -7 25


Key shortcut format:

in a text file add each line for the desired key short cuts in this format:

key filter_index shader_index

an example file would be

	F 75 0
	Q 25 7
If you saved this to keys.key you would use with -k key like his:

	acidcamGL -g -p filters -k keys.key

Playlist file:

use:
	acidcamGL -l

The playlist Now uses String names versus ID identifiers:
to list the different filters
then list them one after the other in a text file line by line like this:

	StrobeEffect
	SelfAlphaBlend

save the file then when in the program press the L key to toggle the playlist on and off and use the arrow keys to move through the list.
To use the playlist file it's the same as the keys just with
Use:

	acidcamGL -L playist.txt -g -p filters

After enabling plugins (used for testing) (with the shell variable pointing to a path) and having the list of plugins in index.txt to load one from the command line use:

	acidcamGL -g -p filters -Z plugin.acidcam

Project comes with some example test plugin files in the acidcamGL/source/plugin folder. To compile these enter the directory and type 

    make

This will install the examples in this tree under acidcamGL/plugin
To export these after compiling them use

    export AC_PLUGIN_PATH=/path/to/acidcamGL/plugin
    
add the export commands to your .bash_profile to not have to issue them every time you start a new session  in the terminal.

To use stereo mode, have two webcams of the same type and use:

    acidcamGL -d camera1_index -6 camera2_index -g -p ./filters -c 1280x720 -r 1280x720 -u 30
    
This will use camera1-index as left eye camera 2 index as right eye  as resolution 1280x720 30 fps

You can also turn a regular video into stereo mode by using the -x argument

    acidcamGL -i file.mp4 -g -x

Or press the '1' key to toggle stereo on and off.

Press < and > to shuffle through different Color Maps.
