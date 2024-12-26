#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec2 glitchOffset(vec2 uv, float intensity) {
    float glitchX = rand(uv) * intensity * sin(time_f);
    float glitchY = rand(uv + vec2(1.0, 1.0)) * intensity * cos(time_f);
    return uv + vec2(glitchX, glitchY);
}

void main() {
    vec2 uv = tc;
    float blockSize = 0.1;
    vec2 blockCoord = floor(uv / blockSize);
    vec2 localCoord = fract(uv / blockSize);
    float randomSeed = rand(blockCoord);
    if (randomSeed > 0.5) {
        uv = glitchOffset(uv, 0.05);
    }
    vec3 texColor = texture(samp, uv).rgb;
    vec3 glitchColor = texColor * (1.0 + 0.5 * sin(time_f * 10.0 + randomSeed * 6.28));
    color = mix(vec4(texColor, 1.0), vec4(glitchColor, 1.0), 0.5);
}

