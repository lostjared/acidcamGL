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
    vec2 tc_centered = tc - center;
    float dist = length(tc_centered);
   vec2 dir = tc_centered / max(dist, 1e-6);
    float waveLength = 0.05;
    float amplitude = 0.02;
    float speed = 2.0;

    float ripple = sin((dist / waveLength - time_f * speed) * 6.2831853); // 2 * PI
    vec2 tc_displaced = tc + dir * ripple * amplitude;
    vec4 texColor = texture(samp, tc_displaced);
    color = texColor;
}

