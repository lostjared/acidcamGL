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

    vec2 uv = tc * 2.0 - 1.0;
    float zigzagAmount = 0.1;
    uv.x += pingPong(uv.y * 10.0 + time_t, 1.0) * zigzagAmount;
    uv.y += pingPong(uv.x * 10.0 + time_t, 1.0) * zigzagAmount;

    float radius = length(uv);
    float angle = atan(uv.y, uv.x);
    float swirlStrength = 3.0;
    angle += swirlStrength * exp(-radius * 2.0) * time_t;

    uv = vec2(cos(angle), sin(angle)) * radius;
    uv = (uv + 1.0) / 2.0;
    uv = clamp(uv, 0.0, 1.0);
    vec4 texColor = texture(samp, uv);
    color = texColor;
}
