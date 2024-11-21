#version 330 core

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

vec2 recursivePattern(vec2 uv, float scale, float iterations) {
    for (float i = 0.0; i < iterations; i++) {
        uv = abs(uv) - 0.5;
        uv = fract(uv * scale) - 0.5;
        uv *= mat2(cos(time_f), -sin(time_f), sin(time_f), cos(time_f));
    }
    return uv;
}

void main() {
    vec2 uv = tc * 2.0 - 1.0;
    uv *= iResolution.y / iResolution.x;
    float angle = atan(uv.y, uv.x);
    float radius = length(uv);
    float segments = 8.0;
    angle = mod(angle, 2.0 * 3.14159 / segments) * segments;
    vec2 mirroredUV = vec2(cos(angle), sin(angle)) * radius;
    vec2 fractalUV = recursivePattern(mirroredUV, 2.5, 5.0);
    vec2 warpedUV = fractalUV + 0.1 * vec2(sin(time_f * 3.0), cos(time_f * 2.0));
    warpedUV += 0.05 * sin(radius * 20.0 - time_f * 5.0);
    vec4 texColor = texture(samp, warpedUV * 0.5 + 0.5);
    vec3 gradient = vec3(0.5 + 0.5 * sin(angle * 5.0 + time_f), 0.5 + 0.5 * cos(radius * 5.0 - time_f), 0.5 + 0.5 * sin(angle * 5.0 - time_f));
    vec3 finalColor = texColor.rgb * gradient;
    float glow = exp(-10.0 * radius);
    finalColor += vec3(glow * 0.8, glow * 0.5, glow);
    color = vec4(finalColor, texColor.a);
    color = mix(color, texture(samp, tc), 0.5);
}

