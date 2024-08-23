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
    vec2 center = iResolution / 2.0;
    vec2 uv = tc * iResolution;
    float distanceFromCenter = length(uv - center);

    float rippleSpeed = 2.0;
    float rippleFrequency = 20.0;

    float time = time_f * rippleSpeed;
    float wave = sin(pingPong(distanceFromCenter + time, rippleFrequency));

    float warpStrength = 0.05;
    float warpFrequency = 5.0;
    vec2 warp = vec2(sin(uv.y * warpFrequency + time), sin(uv.x * warpFrequency + time)) * warpStrength;

    vec2 rippleUV = tc + normalize(uv - center) * wave * 0.01 + warp;

    color = texture(samp, rippleUV);
}
