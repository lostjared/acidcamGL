#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec2 resizeVertical(vec2 uv, float blockSize, float timeFactor) {
    float blockHeight = blockSize;
    float offset = sin(time_f * timeFactor + uv.x * 10.0) * 0.05;
    uv.y += step(mod(uv.y, blockHeight), blockHeight * 0.5) * offset;
    return fract(uv);
}

vec2 randomShift(vec2 uv, float intensity) {
    float shiftX = rand(uv) * intensity * cos(time_f * 2.0);
    float shiftY = rand(uv + vec2(1.0, 1.0)) * intensity * sin(time_f * 2.0);
    return uv + vec2(shiftX, shiftY);
}

void main() {
    vec2 uv = tc;
    float blockSize = 0.1;
    uv = resizeVertical(uv, blockSize, 5.0);
    if (rand(floor(uv / blockSize)) > 0.5) {
        uv = randomShift(uv, 0.02);
    }
    vec4 texColor = texture(samp, fract(uv));
    vec3 glitchColor = texColor.rgb * (1.0 + 0.3 * sin(time_f * 10.0 + uv.y * 50.0));
    color = mix(texColor, vec4(glitchColor, texColor.a), 0.5);
}

