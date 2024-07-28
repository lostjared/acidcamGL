#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec2 uv = tc;
    float glitchFactorX = sin(time_f * 10.0 + uv.y * 20.0) * 0.1;
    float glitchFactorY = cos(time_f * 7.0 + uv.x * 15.0) * 0.1;
    uv.x += glitchFactorX;
    uv.y += glitchFactorY;

    float glitchLine = fract(sin(dot(uv * time_f, vec2(12.9898, 78.233))) * 43758.5453);
    if (glitchLine < 0.1) {
        uv.x += sin(time_f * 50.0) * 0.02;
    } else if (glitchLine < 0.2) {
        uv.y += cos(time_f * 50.0) * 0.02;
    }

    float glitchBlock = fract(sin(dot(uv * time_f, vec2(39.3468, 11.135))) * 23454.2345);
    if (glitchBlock < 0.15) {
        uv.x = mod(uv.x * 5.0, 1.0);
        uv.y = mod(uv.y * 5.0, 1.0);
    }

    vec2 uvOffset = vec2(sin(time_f * 20.0 + uv.y * 30.0), cos(time_f * 20.0 + uv.x * 30.0)) * 0.03;
    uv += uvOffset;

    color = texture(samp, uv);
}
