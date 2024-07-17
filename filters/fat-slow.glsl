#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec2 center = vec2(0.5, 0.5);
    vec2 dir = tc - center;
    float stretch_factor = 1.0 + 0.5 * abs(sin(time_f * 3.14));
    dir.x /= stretch_factor;
    vec2 new_tc = center + dir;
    color = texture(samp, new_tc);
}
