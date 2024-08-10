#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float noise(float x) {
    return fract(sin(x) * 43758.5453);
}

void main(void) {
    vec2 uv = tc;

    vec2 center = uv - 0.5;
    uv = uv + center * (0.02 * sin(time_f * 1.5));

    float timeBasedNoise = noise(time_f * 10.0) * 0.05;
    float spatialNoise = noise(uv.y * 10.0 + time_f * 5.0) * 0.05;
    float combinedNoise = timeBasedNoise + spatialNoise;

    vec3 texColor = texture(samp, uv).rgb;
    float gray = dot(texColor, vec3(0.299, 0.587, 0.114));
    texColor = vec3(gray);

    texColor = texColor * (1.0 - combinedNoise);

    color = vec4(texColor, 1.0);
}
