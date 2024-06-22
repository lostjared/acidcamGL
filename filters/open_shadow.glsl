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
    float trailLength = 1.0;
    float trailOffset = 0.05 * pingPong(time_f, trailLength);
    vec2 offset = vec2(sin(time_f), cos(time_f)) * trailOffset;
    vec4 shadowColor = texture(samp, tc - offset);
    vec4 currentColor = texture(samp, tc);
    color = mix(currentColor, shadowColor, 0.5);
}
