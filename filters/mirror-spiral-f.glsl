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
    vec2 uv = tc * iResolution.xy;
    vec2 center = iResolution * 0.5;
    vec2 offset = uv - center;
    float radius = length(offset);
    float angle = atan(offset.y, offset.x);
    float spiralFactor = sin(time_f + radius * 10.0) * 0.5 + 0.5;
    radius *= spiralFactor;
    float time_t = pingPong(time_f, 25.0) + 2.0;
    vec2 newUV = vec2(
        center.x + radius * cos(angle * time_t),
        center.y + radius * sin(angle * time_t)
    ) / iResolution;
    newUV = mod(newUV, 1.0);
    newUV = abs(newUV);
    color = texture(samp, newUV);
}

