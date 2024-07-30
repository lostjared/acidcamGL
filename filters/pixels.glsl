#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec2 uv = tc;
    vec2 pixelSize = vec2(0.01, 0.01);
    uv = floor(uv / pixelSize) * pixelSize;
    color = texture(samp, uv);
}
