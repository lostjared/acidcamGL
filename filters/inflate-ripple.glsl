#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    float cycle_time = mod(time_f, 5.0);
    float inflate = min(cycle_time * 0.4, 1.5);
    vec2 center = vec2(0.5, 0.5);
    float distance = length(tc - center);
    float ripple = sin((distance - cycle_time) * 10.0) * 0.02;

    vec2 adjusted_tc = (tc - center) * inflate + center + normalize(tc - center) * ripple;
    color = texture(samp, adjusted_tc);
}
