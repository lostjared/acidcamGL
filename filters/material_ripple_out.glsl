#version 330

in vec2 tc;
out vec4 color;
uniform float time_f;
uniform sampler2D samp;
uniform sampler2D mat_samp;
uniform vec2 iResolution;

void main(void) {
    vec2 normPos = (gl_FragCoord.xy / iResolution.xy) * 2.0 - 1.0;
    float dist = length(normPos);
    float phase = sin(dist * 10.0 - time_f * 4.0);
    vec2 tcAdjusted = tc + (normPos * 0.305 * phase);
    vec4 color_samp = texture(samp, tcAdjusted);
    vec4 color_mat_samp = texture(mat_samp, tcAdjusted);
    color = mix(color_samp, color_mat_samp, 0.5);
}