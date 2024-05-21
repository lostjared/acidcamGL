#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

float rand(float seed) {
    return fract(sin(seed) * 43758.5453123);
}

vec2 randomPos(float seed) {
    return vec2(rand(seed), rand(seed + 1.0));
}

vec2 expand(vec2 pos, vec2 center) {
    float t = mod(time_f, 4.0);
    float scale = abs(sin(t * 3.14159 / 2.0));
    return mix(center, pos, scale);
}

void main(void) {
    float cycle = floor(time_f / 4.0);
    vec2 center = randomPos(cycle);
    vec2 pos = expand(tc, center);
    vec4 tcolor = texture(samp, pos);
    color = tcolor;
}
