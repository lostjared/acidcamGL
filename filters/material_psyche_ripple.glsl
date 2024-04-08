#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform sampler2D mat_samp;
uniform vec2 iResolution;
uniform float time_f;

void main(void) {
    vec2 normPos = (gl_FragCoord.xy / iResolution.xy) * 2.0 - 1.0;
    float dist = length(normPos);
    float phase = sin(dist * 10.0 - time_f * 4.0);
    vec2 tcAdjusted = tc + (normPos * 0.305 * phase);
    vec4 color1 = texture(samp, tcAdjusted);
    vec2 centeredCoord = (tc * 2.0) - vec2(1.0, 1.0);
    float stretchFactor = 1.0 + (1.0 - abs(centeredCoord.y)) * 0.5;
    centeredCoord.x *= sin(stretchFactor * time_f);
    vec2 stretchedCoord = (centeredCoord + vec2(1.0, 1.0)) / 2.0;
    vec4 color2 = texture(mat_samp, stretchedCoord);
    color = mix(color1, color2, 0.5);
}
