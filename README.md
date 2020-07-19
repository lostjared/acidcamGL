# acidcamGL

This project is still in very early stages of development.
This project is an implementation of Acid Cam using OpenGL/Shaders.
It is designed more for visualizations or live streaming with OBS versus a Video Editor like the other versions of Acid Cam.
The program is controlled with the PC keyboard.

Arguments:

	-h use h264 output
	-o output mp4 filename
	-k shortcut-key file
	-L playlist of filters
	-b restore black
	-g output debug strings
	-u fps
	-n print filter name
	-e snapshot prefix
	-p shader path
	-f fullscreen
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
