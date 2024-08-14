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
    float angle = atan(tc.y - 0.5, tc.x - 0.5);
    float modulatedTime = pingPong(time_f, 5.0);
    angle += modulatedTime;

    vec2 rotatedTC;
    rotatedTC.x = cos(angle) * (tc.x - 0.5) - sin(angle) * (tc.y - 0.5) + 0.5;
    rotatedTC.y = sin(angle) * (tc.x - 0.5) + cos(angle) * (tc.y - 0.5) + 0.5;

    color = texture(samp, rotatedTC);
}
