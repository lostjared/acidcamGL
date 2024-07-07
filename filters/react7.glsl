#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;

void main(void) {
    vec2 redOffset = vec2(sin(time_f * 5.0), cos(time_f * 5.0)) * 0.02;
    vec2 greenOffset = vec2(cos(time_f * 7.0), sin(time_f * 7.0)) * 0.02;
    vec2 blueOffset = vec2(sin(time_f * 3.0), cos(time_f * 3.0)) * 0.02;

    vec4 redChannel = texture(samp, tc + redOffset);
    vec4 greenChannel = texture(samp, tc + greenOffset);
    vec4 blueChannel = texture(samp, tc + blueOffset);

    color = vec4(redChannel.r, greenChannel.g, blueChannel.b, 1.0);
}
