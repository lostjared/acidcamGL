#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution; 
uniform float time_f;
void main(void) {
    vec2 normCoord = tc;
    float waveAmplitude = 0.15;
    float waveFrequency = 15.0;
    float waveSpeed = 8.0;
    float phase = sin(time_f * 0.5) * 3.14;
    float yWave = sin((normCoord.x + phase) * waveFrequency - (time_f * waveSpeed)) * waveAmplitude;

    vec2 wavedCoord = vec2(normCoord.x, normCoord.y + yWave);
    color = texture(samp, wavedCoord);
}
