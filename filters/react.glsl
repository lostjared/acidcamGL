#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;

void main(void) {
    vec2 uv = tc;
    float amplitude = sin(time_f) * 0.5 + 0.5;
    vec2 distortedUV = uv + vec2(sin(uv.y * 10.0 + time_f) * 0.1, cos(uv.x * 10.0 + time_f) * 0.1) * amplitude;
    color = texture(samp, distortedUV);
}
