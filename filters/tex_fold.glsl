#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    float zoomFactor = 0.5 + 0.5 * sin(time_f);
    float angle = time_f;
    vec2 center = vec2(0.5, 0.5);
    vec2 dir = tc - center;
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    vec2 rotatedTc = vec2(
        cosAngle * dir.x - sinAngle * dir.y,
        sinAngle * dir.x + cosAngle * dir.y
    ) * zoomFactor + center;
    color = texture(samp, rotatedTc);
}
