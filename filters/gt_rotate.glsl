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
    float angle = time_f;
    vec2 tc_centered = tc - center;
    float cos_theta = cos(angle);
    float sin_theta = sin(angle);
    vec2 tc_rotated;
    tc_rotated.x = tc_centered.x * cos_theta - tc_centered.y * sin_theta;
    tc_rotated.y = tc_centered.x * sin_theta + tc_centered.y * cos_theta;

    tc_rotated += center;
    vec4 texColor = texture(samp, tc_rotated);
    color = texColor;
}

