#version 330
in vec2 tc;
out vec4 color;
uniform float time_f;
uniform sampler2D samp;
uniform vec2 iResolution;

void main(void) {
    float yPos = gl_FragCoord.y / iResolution.y;
    float wavePhase = sin(yPos * 10.0 + time_f * 2.0);
    vec2 tcAdjusted = tc + vec2(0, wavePhase * 0.305);
    color = texture(samp, tcAdjusted);
}
