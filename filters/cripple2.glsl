#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec2 uv = tc;
    vec2 center = iResolution * 0.5;
    float distance = length(uv * iResolution - center);
    distance = sin(distance * time_f);
    float ripple = 0.05 * cos(distance * 10.0 - time_f * 5.0);
    uv += ripple;
    color = texture(samp, uv);
}
