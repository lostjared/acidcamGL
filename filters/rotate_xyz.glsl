#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

mat4 rotationMatrixX(float angle) {
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, cos(angle), -sin(angle), 0.0,
        0.0, sin(angle), cos(angle), 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 rotationMatrixY(float angle) {
    return mat4(
        cos(angle), 0.0, sin(angle), 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sin(angle), 0.0, cos(angle), 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 rotationMatrixZ(float angle) {
    return mat4(
        cos(angle), -sin(angle), 0.0, 0.0,
        sin(angle), cos(angle), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

void main(void) {
    vec2 center = vec2(0.5, 0.5);
    vec2 pos = tc - center;

    float angleX = time_f * 0.1;
    float angleY = time_f * 0.2;
    float angleZ = time_f * 0.3;

    mat4 rotationX = rotationMatrixX(angleX);
    mat4 rotationY = rotationMatrixY(angleY);
    mat4 rotationZ = rotationMatrixZ(angleZ);

    vec4 rotatedPos = rotationZ * rotationY * rotationX * vec4(pos, 0.0, 1.0);

    vec2 rotatedTc = rotatedPos.xy + center;

    color = texture(samp, rotatedTc);
}
