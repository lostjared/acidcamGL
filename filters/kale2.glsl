#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;
void main(void) {
    vec2 uv = tc * iResolution / vec2(iResolution.y);
    vec2 center = vec2(0.5, 0.5) * iResolution / vec2(iResolution.y);
    vec2 pos = uv - center;
    float r = length(pos);
    float angle = atan(pos.y, pos.x);
    float numSegments = 6.0;
    float segmentAngle = 2.0 * 3.14159 / numSegments;
    angle = mod(angle, segmentAngle);
    angle = abs(angle - segmentAngle / 2.0);
    pos.x = r * cos(angle);
    pos.y = r * sin(angle);
    float rotationSpeed = time_f * 0.7;
    float cosA = cos(rotationSpeed);
    float sinA = sin(rotationSpeed);
    mat2 rot = mat2(cosA, -sinA, sinA, cosA);
    pos = rot * pos;
    vec4 texColor = texture(samp, sin(pos * time_f) + center);
    color = texColor;
}
