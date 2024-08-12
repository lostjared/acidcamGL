#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

void main(void) {
    float time_t = pingPong(time_f, 10.0);
    
    float dist = sqrt(tc.x * tc.x + tc.y * tc.y);
    dist = sin(dist * time_t);
    float ripple = sin(dist * 10.0 - time_f * 5.0);
    vec2 offset = tc + ripple * 0.02;
    vec4 texColor = texture(samp, offset);
    color = texColor;
}
