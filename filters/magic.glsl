#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec2 uv = tc;
    float x = uv.x + sin(uv.y * 10.0 + time_f * 5.0) * 0.05;
    float y = uv.y + cos(uv.x * 10.0 + time_f * 5.0) * 0.05;
    vec2 displacedUV = vec2(x, y);
    vec4 texColor = texture(samp, displacedUV);
    float haze = sin(uv.y * 20.0 + time_f * 10.0) * 0.5 + 0.5;
    vec4 hazeColor = vec4(0.8, 0.3, 1.0, 1.0) * haze;
    color = texColor + hazeColor;
}
