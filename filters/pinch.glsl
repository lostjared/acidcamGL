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
    vec2 uv = tc;
    vec2 center = vec2(0.5, 0.5);
    uv -= center;
    float dist = length(uv);
    float time_t = pingPong(time_f, 10.0);
    float pinchAmount = 1.5 * sin(time_t);
    float radius = 0.5;
    if (dist < radius) {
        uv *= 1.0 - (pinchAmount * (1.0 - dist / radius));
    }
    
    uv += center;
    color = texture(samp, uv);
}
