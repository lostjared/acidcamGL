#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;

void main(void) {
    vec4 baseColor = texture(samp, tc);
    float glitchOffsetX = fract(sin(dot(tc, vec2(12.9898, 78.233))) * 43758.5453) * 0.1 * sin(time_f * 20.0);
    float glitchOffsetY = fract(cos(dot(tc, vec2(4.898, 7.23))) * 23421.6312) * 0.1 * cos(time_f * 20.0);
    vec2 glitchTc = tc + vec2(glitchOffsetX, glitchOffsetY);
    vec4 glitchColor = texture(samp, glitchTc);
    float glitchStrength = 0.5 + 0.5 * cos(time_f * 5.0);
    color = mix(baseColor, glitchColor, glitchStrength);
}
