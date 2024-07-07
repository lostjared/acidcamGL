#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;

void main(void) {
    vec4 baseColor = texture(samp, tc);
    float glitchOffsetX = sin(time_f * 10.0 + tc.y * 20.0) * 0.05;
    float glitchOffsetY = cos(time_f * 15.0 + tc.x * 25.0) * 0.05;
    vec2 glitchTc = tc + vec2(glitchOffsetX, glitchOffsetY);
    vec4 glitchColor = texture(samp, glitchTc);
    float glitchStrength = 0.5 + 0.5 * sin(time_f * 3.0);
    color = mix(baseColor, glitchColor, glitchStrength);
}
