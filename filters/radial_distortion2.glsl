#version 330 core

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

void main(void) {
    vec2 uv = tc - 0.5;
    float len = length(uv);
    float time_t = pingPong(time_f, 2.0) + 0.05;
    float radialDistortion = sin(len * 10.0 - time_f) * time_t;
    uv += uv * radialDistortion;
    float warpFactor = sin(time_f) * 0.5 + 0.5;
    vec2 warpedCoords;
    warpedCoords.x = pingPong(uv.x + time_f * 0.1, 1.0);
    warpedCoords.y = pingPong(uv.y + time_f * 0.1, 1.0);

    color = texture(samp, warpedCoords + 0.5); //
}

