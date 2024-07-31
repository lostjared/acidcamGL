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
    float time_t = pingPong(time_f, 10.0) + 1.0;
    float time_s = pingPong(time_f, 5.0);
    float frequency = 10.0;
    float amplitude = sin(0.02 * time_t);
    float speed = sin(0.3 * time_s);

    vec2 uv = tc;
    uv.y += sin(uv.x * frequency + time_f * speed) * amplitude;

    color = texture(samp, uv);
}
