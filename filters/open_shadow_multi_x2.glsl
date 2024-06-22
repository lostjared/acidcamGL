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
    float trailLength = 16.0;
    vec4 currentColor = texture(samp, tc);

    vec4 shadowColor1 = texture(samp, tc - vec2(sin(time_f), cos(time_f)) * 0.02 * pingPong(time_f, trailLength));
    vec4 shadowColor2 = texture(samp, tc - vec2(sin(time_f + 1.0), cos(time_f + 1.0)) * 0.04 * pingPong(time_f + 1.0, trailLength));
    vec4 shadowColor3 = texture(samp, tc - vec2(sin(time_f + 2.0), cos(time_f + 2.0)) * 0.06 * pingPong(time_f + 2.0, trailLength));

    vec4 shadowColor = (shadowColor1 + shadowColor2 + shadowColor3) / 3.0;

    color = mix(currentColor, shadowColor, 0.5);
}
