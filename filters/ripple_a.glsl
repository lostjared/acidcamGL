
#version 330 core

out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

const float PI = 3.14159265359;
const vec3 NEON_COLOR = vec3(0.8, 0.2, 1.0);

float pingPong(float value, float length) {
    float modValue = mod(value, length * 2.0);
    return length - abs(modValue - length);
}

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 vortexEffect(vec2 uv, float time) {
    uv = uv * 2.0 - 1.0;
    uv.x *= iResolution.x / iResolution.y;
    float angle = atan(uv.y, uv.x) + time * 0.5;
    float radius = length(uv);
    uv = vec2(cos(angle), sin(angle)) * radius;
    float ripple = sin(radius * 10.0 - time * 5.0) * 0.05;
    uv += ripple * vec2(cos(angle), sin(angle));
    vec3 texColor = texture(samp, uv * 0.5 + 0.5).rgb;
    texColor += NEON_COLOR * exp(-10.0 * radius);
    return texColor;
}

void main(void) {
    vec2 uv = tc;
    vec3 colorEffect = vortexEffect(uv, time_f);
    float offset = 0.005 * sin(time_f);
    vec3 finalColor = vec3(
        vortexEffect(uv + vec2(offset, 0.0), time_f).r,
        colorEffect.g,
        vortexEffect(uv - vec2(offset, 0.0), time_f).b
    );
    color = vec4(finalColor, 1.0);
}
