#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

void main(void) {
    vec2 normCoord = (tc * 2.0 - 1.0) * vec2(iResolution.x / iResolution.y, 1.0);
    float dist = length(normCoord);
    float cycleDuration = 10.0;
    float cyclePhase = mod(time_f, cycleDuration) / cycleDuration;
    float effectPhase = cyclePhase < 0.5 ? cyclePhase * 2.0 : (1.0 - cyclePhase) * 2.0;
    float effectRadius = 0.75 * sin(effectPhase * 3.14159);
    if (dist < effectRadius) {
        float effectStrength = 10.0 - (dist / effectRadius);
        normCoord *= 1.0 + effectStrength * (effectRadius - dist) * sin(effectPhase * 3.14159);
    }
    vec2 finalCoord = (normCoord / vec2(iResolution.x / iResolution.y, 1.0) + 1.0) / 2.0;
    color = texture(samp, finalCoord);
}
