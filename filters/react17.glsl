#version 330 core
in vec2 tc;
out vec4 color;
uniform float time_f;
uniform sampler2D samp;
uniform vec2 iResolution;

void main(void) {
    vec2 normPos = gl_FragCoord.xy / iResolution.xy;
    float cycle = sin(time_f * 0.5);
    float movingPhase = normPos.x + cycle;
    float phase = sin(movingPhase * 10.0 - time_f * 2.0);
    vec2 tcAdjusted = tc + (vec2(phase, 0) * 0.302);

    float glitchFactor = sin(time_f);
    vec2 glitchOffset = vec2(glitchFactor * 0.1, glitchFactor * 0.1);
    vec4 glitchColor = texture(samp, tc + glitchOffset);

    vec4 baseColor = texture(samp, tcAdjusted);
    color = mix(baseColor, glitchColor, 0.5);
}






