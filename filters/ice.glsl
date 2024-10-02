#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float noise(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898,78.233))) * 43758.5453);
}

float smoothNoise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);

    float a = noise(i);
    float b = noise(i + vec2(1.0, 0.0));
    float c = noise(i + vec2(0.0, 1.0));
    float d = noise(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b - c + a) * u.x * u.y;
}

float fbm(vec2 p) {
    float total = 0.0;
    float amplitude = 1.0;
    float frequency = 1.0;

    for(int i = 0; i < 5; i++) {
        total += smoothNoise(p * frequency) * amplitude;
        amplitude *= 0.5;
        frequency *= 2.0;
    }

    return total;
}

void main(void) {
    float distortionStrength = 0.02;
    vec2 distortion = vec2(
        fbm(tc * 10.0 + time_f) - 0.5,
        fbm(tc * 10.0 - time_f) - 0.5
    );

    vec2 distortedTC = tc + distortion * distortionStrength;

    color = texture(samp, distortedTC);
}
