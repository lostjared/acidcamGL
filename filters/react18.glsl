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
    float glitchStrength = 0.05;
    vec2 glitch = vec2(
        pingPong(time_f * 10.0 + tc.y * 20.0, 1.0) * glitchStrength,
        pingPong(time_f * 10.0 + tc.x * 20.0, 1.0) * glitchStrength
    );
    vec2 displacedTc = tc + glitch;
    color = texture(samp, displacedTc);
}
