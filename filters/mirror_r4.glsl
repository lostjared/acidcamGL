#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

mat2 rotate(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat2(c, -s, s, c);
}

vec2 mirror(vec2 uv) {
    return abs(mod(uv, 2.0) - 1.0);
}

void main() {
    vec2 uv = tc - 0.5;
    uv = rotate(time_f * 0.5) * uv + 0.5;
    vec2 sectionSize = iResolution / vec2(2.0, 2.0);
    vec2 center = (floor(uv * vec2(2.0, 2.0)) + 0.5) * sectionSize / iResolution;
    uv = (uv - center) * iResolution.xy / sectionSize;
    float direction = mod(floor(center.x * 2.0) + floor(center.y * 2.0), 2.0) * 2.0 - 1.0;
    uv = rotate(direction * (time_f + length(center))) * uv;
    uv = mirror(uv);
    vec2 texCoord = (uv + 0.5) * sectionSize / iResolution + center;
    color = texture(samp, texCoord);
}

