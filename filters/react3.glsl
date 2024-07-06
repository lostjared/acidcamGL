#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;

void main(void) {
    vec2 uv = tc - 0.5;
    float radius = length(uv);
    float angle = atan(uv.y, uv.x) + radius * 10.0 + time_f * (0.5 + sin(time_f) * 0.5);
    vec2 spiralUV = vec2(cos(angle), sin(angle)) * radius;
    color = texture(samp, spiralUV + 0.5);
}
