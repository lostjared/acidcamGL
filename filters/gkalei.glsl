#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

float noise(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

float smoothNoise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    float a = noise(i);
    float b = noise(i + vec2(1.0, 0.0));
    float c = noise(i + vec2(0.0, 1.0));
    float d = noise(i + vec2(1.0, 1.0));
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

float fractalNoise(vec2 p) {
    float value = 0.0;
    float amplitude = 0.5;
    for (int i = 0; i < 4; i++) {
        value += amplitude * smoothNoise(p);
        p *= 2.0;
        amplitude *= 0.5;
    }
    return value;
}

vec3 hsvToRgb(float h, float s, float v) {
    vec3 p = abs(fract(vec3(5.0, 3.0, 1.0) + h * 6.0) * 2.0 - 1.0);
    return v * mix(vec3(1.0), clamp((p - 1.0 + s), 0.0, 1.0), s);
}

void main(void) {
    vec2 uv = tc * 2.0 - vec2(1.0);
    uv *= iResolution.x / iResolution.y;
    uv *= 2.0;

    float displacement = fractalNoise(uv * 5.0 + time_f * 0.5);
    vec2 dispCoords = tc + vec2(displacement * 0.05, displacement * 0.1);
    vec4 texColor = texture(samp, dispCoords);

    float radius = length(uv);
    float angle = atan(uv.y, uv.x) + time_f * 1.5;
    float hue = mod(angle / (2.0 * 3.14159) + time_f * 0.3, 1.0);
    
    vec3 prismColor = hsvToRgb(hue, 1.0, 1.0 - pingPong(radius * 0.8 + time_f * 0.5, 1.0));
    color = vec4(sin(prismColor * time_f) * texColor.rgb, texColor.a);
    
}
