#version 330
in vec2 tc;
out vec4 color;
uniform float time_f;
uniform sampler2D samp;
uniform vec2 iResolution;

void main(void) {
    vec2 normPos = (gl_FragCoord.xy / iResolution.xy) * 2.0 - 1.0;
    float dist = length(normPos);
    float phase = sin(dist * 10.0 - time_f * 4.0);
    vec2 tcAdjusted = tc + (normPos * 0.305 * phase);
    float dispersionScale = 0.02;
    vec2 dispersionOffset = normPos * dist * dispersionScale;
    vec2 tcAdjustedR = tcAdjusted + dispersionOffset * (-1.0);
    vec2 tcAdjustedG = tcAdjusted;
    vec2 tcAdjustedB = tcAdjusted + dispersionOffset * 1.0;
    float r = texture(samp, tcAdjustedR).r;
    float g = texture(samp, tcAdjustedG).g;
    float b = texture(samp, tcAdjustedB).b;
    color = vec4(r, g, b, 1.0);
}

