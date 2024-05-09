#version 330 core

out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main() {
    vec2 pos = tc;
    float time_t = mod(time_f, 3.0);
    pos.x = 1.0-tc.x;
    color = mix(texture(samp, tan(tc * time_t)), texture(samp, tan(pos * time_t)), 0.5);
}
