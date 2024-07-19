#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

vec2 glitchEffect(vec2 uv, float timeAmp) {
    float time = floor(timeAmp);
    float amp = fract(timeAmp);
    uv.y += amp * 0.1 * sin(time * 10.0 + uv.x * 100.0);
    uv.x += amp * 0.05 * sin(time * 5.0 + uv.y * 200.0);
    uv.x += rand(vec2(uv.y * time, uv.x)) * 0.1 * amp;
    uv.y += rand(vec2(uv.x * time, uv.y)) * 0.1 * amp;
    return uv;
}

void main(void) {
    vec2 uv = tc;
    uv = glitchEffect(uv, time_f);
    float time_t = pingPong(time_f, 10.0) + 2.0;
    color = sin(texture(samp, uv) * time_t);
}
