#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

void main(void) {
    vec2 uv = tc * iResolution / vec2(iResolution.y);
    vec2 center = vec2(0.5, 0.5) * iResolution / vec2(iResolution.y);
    vec2 pos = (uv - center) * 2.0;
    float angle = time_f;
    float cosA = cos(angle);
    float sinA = sin(angle);
    mat2 rotate = mat2(cosA, -sinA, sinA, cosA);
    pos = rotate * pos;
    float r = length(pos);
    float a = atan(pos.y, pos.x);
    float n = 5.0;
    float t = mod(a + 3.14159 / n, 2.0 * 3.14159 / n) * n / 2.0 / 3.14159;
    t = abs(t - 0.5);
    float time_t = time_f;
    color = texture(samp, sin(vec2(tc[0] * t *  time_t, tc[1] * a * time_t)));
}
