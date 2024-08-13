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
    vec2 uv = tc - 0.5;
    float len = length(uv);
    float time_t = pingPong(time_f, 2.0) + 0.05;
    
    float distortion = sin(len * 10.0 - time_f) * time_t;
    uv += uv * distortion;
    color = texture(samp, uv + 0.5);
}
