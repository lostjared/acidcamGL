#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec2 uv = tc * 2.0 - 1.0;
    uv *= iResolution.x / iResolution.y;
    float r = length(uv);
    float theta = atan(uv.y, uv.x);
    float spiralEffect = time_f * 0.2;
    r -= mod(spiralEffect, 4.0);
    theta += spiralEffect;
    vec2 distorted_uv = vec2(cos(theta), sin(theta)) * r;
    distorted_uv = (distorted_uv / (iResolution.x / iResolution.y)) * 0.5 + 0.5;

    vec4 texColor = texture(samp, distorted_uv);
    color = vec4(texColor.rgb, 1.0);
}
