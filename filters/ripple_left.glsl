#version 330
in vec2 tc;
out vec4 color;
uniform float time_f;
uniform sampler2D samp;
uniform vec2 iResolution;

void main(void) {
    vec2 normTC = gl_FragCoord.xy / iResolution.xy;
    float diagonalDist = length(vec2(normTC.x + normTC.y - 1.0, normTC.x - normTC.y));

    float phase = sin(diagonalDist * 20.0 - time_f * 5.0);
    vec2 tcAdjusted = tc + (normTC * 0.301 * phase);
    color = texture(samp, tcAdjusted);
}
