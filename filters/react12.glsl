#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;

void main(void) {
    vec2 center = vec2(0.5, 0.5);
    vec2 uv = tc - center;
    float dist = length(uv);
    float ripple = sin(10.0 * dist - time_f * 6.28318) * 0.1;
    uv += uv * ripple;
    uv += center;

    color = texture(samp, uv);
}
