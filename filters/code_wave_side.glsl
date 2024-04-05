#version 330
in vec2 tc;
out vec4 color;
uniform float time_f;
uniform sampler2D samp;
uniform vec2 iResolution;

void main(void) {
    float xNorm = (gl_FragCoord.x / iResolution.x) * 2.0 - 1.0;
    float phase = abs(sin(time_f * 2.0));
    vec2 tcAdjusted = tc + vec2(phase * xNorm * 0.305, 0);
    color = texture(samp, tcAdjusted);
}
