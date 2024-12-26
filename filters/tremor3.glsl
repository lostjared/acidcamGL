#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec2 tremorEffect(vec2 uv, float frequency, float maxAmplitude) {
    float amplitude = maxAmplitude * (0.5 + 0.5 * sin(time_f * 2.0));
    float tremorX = sin(uv.y * frequency + time_f * 10.0) * amplitude;
    float tremorY = cos(uv.x * frequency + time_f * 10.0) * amplitude;
    return uv + vec2(tremorX, tremorY);
}

void main() {
    vec2 uv = tc;
    float frequency = 30.0;
    float maxAmplitude = 0.02;
    uv = tremorEffect(uv, frequency, maxAmplitude);
    vec4 texColor = texture(samp, fract(uv));
    color = texColor;
}

