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
    float timeVar = time_f * 0.5;
    vec2 noise = vec2(
        pingPong(tc.x + timeVar, 1.0),
        pingPong(tc.y + timeVar, 1.0)
    );

    float stretchFactorX = 1.0 + 0.3 * sin(time_f + tc.y * 10.0);
    float stretchFactorY = 1.0 + 0.3 * cos(time_f + tc.x * 10.0);
    
    vec2 distortedUV = vec2(
        tc.x * stretchFactorX + noise.x * 0.1,
        tc.y * stretchFactorY + noise.y * 0.1
    );

    color = texture(samp, distortedUV);
}
