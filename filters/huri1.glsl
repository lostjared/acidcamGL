#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

vec2 vortexDistortion(vec2 uv, float time) {
    vec2 center = vec2(0.5, 0.5);
    vec2 offset = uv - center;
    float distance = length(offset);
    float angle = atan(offset.y, offset.x) + sin(time + distance * 10.0) * 
0.5;
    return center + vec2(cos(angle), sin(angle)) * distance;
}

vec2 rotateX(vec2 uv, float angle) {
    float cosA = cos(angle);
    float sinA = sin(angle);
    uv -= 0.5;
    uv = vec2(uv.x, cosA * uv.y - sinA * uv.x);
    uv += 0.5;
    return uv;
}

void main(void) {
    vec2 uv = tc;
    uv = rotateX(uv, time_f);
    uv = vortexDistortion(uv, time_f);
    vec4 texColor = texture(samp, uv);
    color = texColor;
}

