#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

void main(void) {
    float time_t = pingPong(time_f, 7.0);
    vec2 uv = tc - 0.5;
    float distance = length(uv);
    float pulse = 1.0 + 0.5 * sin(time_t * 2.0 + distance * 20.0);
    uv *= pulse;
    uv += sin(0.5 * time_t);
    color = texture(samp, uv);
}
