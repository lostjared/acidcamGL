#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

mat3 rotationMatrixY(float angle) {
    return mat3(
        cos(angle), 0.0, sin(angle),
        0.0,       1.0, 0.0,
       -sin(angle), 0.0, cos(angle)
    );
}

vec2 vortexDistortion(vec2 uv, float time) {
    vec2 center = vec2(0.5, 0.5);
    vec2 offset = uv - center;
    float distance = length(offset);
    float angle = atan(offset.y, offset.x) + sin(time + distance * 10.0) * 0.5;
    return center + vec2(cos(angle), sin(angle)) * distance;
}

void main(void) {
    vec2 uv = tc;

    float angle = time_f * 0.5;
    mat3 rotation = rotationMatrixY(angle);
    uv = (rotation * vec3(uv - 0.5, 1.0)).xy + 0.5;
    uv = vortexDistortion(uv, time_f);
    vec4 texColor = texture(samp, uv);
    color = texColor;
}

