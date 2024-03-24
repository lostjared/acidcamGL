#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform sampler2D mat_samp;
uniform float time_f;

void main(void) {
    float frostAmount = 0.1;
    float frostScale = 3.0;
    vec2 noiseOffset = vec2(time_f * 0.02, time_f * -0.02);
    vec2 noiseTC = tc * frostScale + noiseOffset;
    float noiseValue = texture(mat_samp, noiseTC).r;
    vec2 distortedTC = tc + (noiseValue - 0.5) * frostAmount;

    vec4 sceneColor = texture(samp, distortedTC);

    color = mix(texture(samp, tc), sceneColor, noiseValue * frostAmount);
}
