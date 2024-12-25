#version 330 core

out vec4 color;
in vec2 tc;

uniform vec2 iResolution;
uniform float time_f;
uniform sampler2D samp;

vec2 kaleidoscope(vec2 uv, float segments) {
    float angle = atan(uv.y, uv.x);
    float radius = length(uv);
    angle = mod(angle, 2.0 * 3.14159265359 / segments);
    return vec2(cos(angle), sin(angle)) * radius;
}

float fractalSpiral(vec2 uv) {
    float scale = 1.0;
    float intensity = 0.0;
    float rotation = time_f * 0.5;
    for (int i = 0; i < 8; i++) {
        uv = mat2(cos(rotation), -sin(rotation), sin(rotation), cos(rotation)) * uv;
        uv = abs(uv) / dot(uv, uv) - 0.5;
        intensity += 1.0 / scale * sin(uv.x * 10.0 + uv.y * 10.0 + time_f);
        scale *= 1.8;
    }
    return intensity;
}

vec3 blendMaterials(vec3 texColor, vec3 fractalColor, vec2 uv) {
    float fresnel = pow(1.0 - dot(normalize(uv), vec2(0.0, 0.0)), 2.0);
    vec3 specularHighlight = vec3(1.0) * pow(max(0.0, fractalColor.r), 20.0);
    return mix(texColor, fractalColor + specularHighlight, fresnel);
}

float pingPong(float x, float length) {
    return length - abs(mod(x, 2.0 * length) - length);
}

void main() {
    vec2 uv = tc * 2.0 - 1.0;
    uv.x *= iResolution.x / iResolution.y;
    float segments = 6.0 + mod(time_f, 10.0);
    uv = kaleidoscope(uv, segments);
    float fractalValue = fractalSpiral(uv * 2.0);
    vec3 fractalColor = vec3(fractalValue * 0.8, fractalValue * 0.6, fractalValue * 0.4);
    vec3 texColor = texture(samp, uv * 0.5 + 0.5).rgb;
    vec3 blendedColor = blendMaterials(texColor, fractalColor, uv);
    blendedColor += vec3(0.3);
    blendedColor.r += sin(uv.x * 2.0 + time_f) * 0.2;
    blendedColor.g += sin(uv.y * 2.0 + time_f * 0.5) * 0.2;
    blendedColor.b += sin((uv.x + uv.y) * 0.5 + time_f) * 0.2;
    color = vec4(blendedColor, 1.0);
    color = mix(color, texture(samp, tc), 0.5);
    float time_t = pingPong(time_f, 5.0) + 1.5;
    color = color * sin(vec4(blendedColor, 1.0) * time_t);
}
