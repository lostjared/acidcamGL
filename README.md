# acidcamGL

This project is still in very early stages of development.
This project is an implementation of Acid Cam using OpenGL/Shaders.
It is designed more for visualizations or live streaming with OBS versus a Video Editor like the other versions of Acid Cam.
The program is controlled with the PC keyboard.

Arguments:

	-h use H.264 output w/o uses MPEG-4
	-o output mp4 filename
	-k shortcut-key file
	-L playlist of filters
	-b restore black
	-g output debug strings
	-u fps
	-n print filter name
	-e snapshot prefix
	-p shader path
	-M monitor index
	-f fullscreen
	-F force fullscreen in 4K
	-d capture device
	-r resolution 1920x1080
	-c Camera resolution 1280x720
	-l list filters
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
	Q,Q,E,R,Y,U,I,O - move movement rate
	T - reset color offset
	N - Jump to end of filter list

Key shortcut format:

in a text file add each line for the desired keyshort cuts in this format:

key filter_index shader_index

an example file would be

	F 75 0
	Q 25 7

If you saved this to keys.key you would use it with -k key like his:

	./acidcaml -g -p filters -k keys.key

Playlist file:

use:
	./acidcamGL -l

to list the different filters
then list them one after the other in a text file line by line like this:

	2
	4
	19
	28
	1

save the file then when in the program press the L key to toggle the playlist on and off and use the arrow keys to move through the list.
To use the playlist file its he same as the keys just with
use:
	./acid -L playist.txt -g -p filters
