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
    vec2 center = vec2(0.5, 0.5);
    vec2 offset = tc - center;
    float maxRadius = length(vec2(0.5, 0.5));
    float radius = length(offset);
    float normalizedRadius = radius / maxRadius;
    float angle = atan(offset.y, offset.x);

    float distortion = 0.5;
    float distortedRadius = normalizedRadius + distortion * pow(normalizedRadius, 2.0);
    distortedRadius = clamp(distortedRadius, 0.0, 1.0);
    distortedRadius *= maxRadius;
    vec2 distortedCoords = center + distortedRadius * vec2(cos(angle), sin(angle));
    float modulatedTime = pingPong(time_f, 5.0);
    angle += modulatedTime;
    vec2 rotatedTC;
    rotatedTC.x = cos(angle) * (distortedCoords.x - center.x) - sin(angle) * (distortedCoords.y - center.y) + center.x;
    rotatedTC.y = sin(angle) * (distortedCoords.x - center.x) + cos(angle) * (distortedCoords.y - center.y) + center.y;
    vec2 warpedCoords;
    warpedCoords.x = pingPong(rotatedTC.x + time_f * 0.1, 1.0);
    warpedCoords.y = pingPong(rotatedTC.y + time_f * 0.1, 1.0);
    color = texture(samp, warpedCoords);
}

