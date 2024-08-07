
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
    float distance = length(uv - center);
    float time_t = pingPong(time_f, 15.0) + 2.0;
    distance = sin(distance * time_t);
    
    uv.x += tan(distance * 20.0 - time_f * 2.0) * 0.02;
    uv.y += tan(distance * 20.0 - time_f * 2.0) * 0.02;

    color = texture(samp, uv);
}
