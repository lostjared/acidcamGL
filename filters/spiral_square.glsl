#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;


vec2 rotate(vec2 v, float a) {
    float s = sin(a);
    float c = cos(a);
    mat2 m = mat2(c, -s, s, c);
    return m * v;
}

void main() {
    float time_t = mod(time_f, 15.0);
    vec2 uv = tc;
    uv = uv * 2.0 - 1.0;
    uv = rotate(uv, time_t);
    uv += 2.0;
    uv = fract(uv * time_t);
    color = texture(samp, uv);
}
