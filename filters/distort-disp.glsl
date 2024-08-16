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

void main() {
    float displacement = pingPong(tc.x * iResolution.x + time_f * 50.0, 20.0);
    vec2 distortedTC = vec2(
        atan(tc.y - 0.5, tc.x - 0.5) / 3.14159 + 0.5,
        tc.y + sin(time_f * 2.0) * 0.02
    );
    distortedTC.x += displacement / iResolution.x;
    color = texture(samp, sin(distortedTC * time_f));
}
