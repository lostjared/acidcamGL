#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec2 uv = tc;

    float glitchStrength = sin(time_f * 10.0) * 0.1;
    float glitchOffsetX = fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453) * glitchStrength;
    float glitchOffsetY = fract(cos(dot(uv, vec2(4.898, 7.23))) * 23421.6312) * glitchStrength;

    uv.x += glitchOffsetX;
    uv.y += glitchOffsetY;

    vec4 colorA = texture(samp, uv);
    vec4 colorB = texture(samp, uv + vec2(0.01 * sin(time_f * 50.0), 0.01 * cos(time_f * 50.0)));

    float noise = fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);
    color = mix(colorA, colorB, noise);
}
