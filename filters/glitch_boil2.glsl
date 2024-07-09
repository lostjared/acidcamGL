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
    float boil = sin(uv.y * 20.0 + time_f * 6.0) * 0.2 * time_f;
    boil += cos(uv.x * 30.0 + time_f * 4.0) * 0.2 * time_f;
    boil += sin((uv.x + uv.y) * 25.0 + time_f * 5.0) * 0.1 * time_f;
    uv.y += boil;
    uv.x += boil;
    float time_t = pingPong(time_f, 10.0);
    vec4 distorted = texture(samp, sin(uv * time_t));
    vec4 baseTexture = texture(samp, tc);
    color = mix(baseTexture, distorted, 0.5);
}
