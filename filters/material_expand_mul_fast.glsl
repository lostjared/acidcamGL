#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform sampler2D mat_samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    float t = sin(time_f) * 0.5 + 0.5;
    vec2 center = vec2(0.5, 0.5);
    vec2 normTC = tc - center;
    float time_t = mod(sin(time_f * t), 25);
    vec2 squeezed = sin(normTC * time_t) * (1.0 - t) + center;
    vec4 texColor = texture(samp, squeezed);
    color = texColor;
    color = mix(color, texture(mat_samp, tc), 0.5);
}
