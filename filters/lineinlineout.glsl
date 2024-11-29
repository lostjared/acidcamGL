#version 330 core

out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main() {
    vec2 uv = tc;
    float centerY = 0.5 + 0.25 * sin(time_f);
    float shiftAmount = 0.1 * sin(time_f);

    if (uv.y > centerY) {
        uv.x += shiftAmount;
    } else {
        uv.x -= shiftAmount;
    }

    vec4 baseColor = texture(samp, uv);
    color = baseColor;
}
