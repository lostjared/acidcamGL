#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float noise(vec2 pos) {
    vec2 i = floor(pos);
    vec2 f = fract(pos);
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(mix(rand(i + vec2(0.0, 0.0)), rand(i + vec2(1.0, 0.0)), u.x),
               mix(rand(i + vec2(0.0, 1.0)), rand(i + vec2(1.0, 1.0)), u.x), u.y);
}

void main(void) {
    vec2 uv = tc;
    float glitchStrength = 0.01;
    float timeNoise = noise(uv * 10.0 + time_f * 0.5);
    uv.x += (rand(uv + time_f) - 0.5) * glitchStrength;
    uv.y += (rand(uv + time_f * 1.5) - 0.5) * glitchStrength;
    vec4 texColor = texture(samp, uv);
    vec4 colorShift = vec4(texColor.r, texColor.g * 0.5 + 0.5 * timeNoise, texColor.b * 0.5 + 0.5 * (1.0 - timeNoise), texColor.a);
    float glitchNoise = rand(uv + time_f);
    vec4 glitchColor = vec4(vec3(sin(glitchNoise * time_f)), 1.0) * glitchStrength ;
    color = mix(colorShift, glitchColor, glitchStrength * glitchNoise);
    color = sin(color * time_f);
}
