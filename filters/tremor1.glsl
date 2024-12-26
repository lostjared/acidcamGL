#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec2 tremorEffect(vec2 uv, float frequency, float amplitude) {
    float tremorX = sin(uv.y * frequency + time_f * 5.0) * amplitude;
    float tremorY = cos(uv.x * frequency + time_f * 5.0) * amplitude;
    return uv + vec2(tremorX, tremorY);
}

void main() {
    vec2 uv = tc;
    float frequency = 20.0;
    float amplitude = 0.01 + 0.01 * sin(time_f * 3.0);
    uv = tremorEffect(uv, frequency, amplitude);
    vec4 texColor = texture(samp, fract(uv));
    color = texColor;
}

