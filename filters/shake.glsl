#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;
uniform float value_alpha_r, value_alpha_g, value_alpha_b;

void main(void) {
    vec2 pos = tc;
    pos = pos + value_alpha_r;
    color = texture(samp, pos);
}
