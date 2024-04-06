#version 330 core
out vec4 color;
in vec2 tc;
uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;
void main(void) {
    vec2 normCoord = (tc * 2.0 - 1.0) * vec2(iResolution.x / iResolution.y, 1.0);
    float waveStrength = 0.05;
    float waveFrequency = 3.0;
    vec2 wave = vec2(sin(normCoord.y * waveFrequency + time_f) * waveStrength,
                     cos(normCoord.x * waveFrequency + time_f) * waveStrength);

    normCoord += wave;
    float dist = length(normCoord);
    float angle = atan(normCoord.y, normCoord.x);\
    float spiralAmount = tan(time_f) * 3.0;
    angle += dist * spiralAmount;
    vec2 spiralCoord = vec2(cos(angle), sin(angle)) * dist;
    spiralCoord = (spiralCoord / vec2(iResolution.x / iResolution.y, 1.0) + 1.0) / 2.0;
    color = texture(samp, spiralCoord);
}
