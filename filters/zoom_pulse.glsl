#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec2 uv = tc - 0.5;
    float distance = length(uv);
    float pulse = 1.0 + 0.5 * sin(time_f * 2.0 + distance * 20.0);
    uv *= pulse;
    uv += 0.5;
    color = texture(samp, uv);
}
