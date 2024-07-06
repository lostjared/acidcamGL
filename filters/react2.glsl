#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;

void main(void) {
    vec2 uv = tc - 0.5;
    float amplitude = sin(time_f) * 0.5 + 0.5;
    float angle = time_f + amplitude * 10.0;
    mat2 rotation = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
    vec2 rotatedUV = rotation * uv;
    color = texture(samp, rotatedUV + 0.5);
}
