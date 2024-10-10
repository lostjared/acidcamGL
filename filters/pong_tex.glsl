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
    vec2 uv = tc * 2.0;
    uv.x = pingPong(uv.x, 1.0);
    uv.y = pingPong(uv.y, 1.0);
    float frequency = 10.0;
    float amplitude = 0.05;
    uv += sin(uv * frequency + time_f) * amplitude;
    vec4 texColor = texture(samp, uv);
    color = texColor;
}
