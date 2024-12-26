#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec2 tremorEffect(vec2 uv, float baseFrequency, float baseAmplitude, float timeFactor) {
    float duration = mod(time_f, timeFactor);
    float frequency = baseFrequency * (1.0 + 0.5 * duration / timeFactor);
    float amplitude = baseAmplitude * (1.0 + 0.5 * duration / timeFactor);
    float tremorX = sin(uv.y * frequency + time_f * 10.0) * amplitude;
    float tremorY = cos(uv.x * frequency + time_f * 10.0) * amplitude;
    return uv + vec2(tremorX, tremorY);
}

void main() {
    vec2 uv = tc;
    float baseFrequency = 20.0;
    float baseAmplitude = 0.01;
    float timeFactor = 5.0;
    uv = tremorEffect(uv, baseFrequency, baseAmplitude, timeFactor);
    vec4 texColor = texture(samp, fract(uv));
    color = texColor;
}

