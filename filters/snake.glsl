#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;
void main() {
    vec2 uv = tc * iResolution;
    float wave = sin(uv.x * 0.05 + time_f * 2.0) * 0.05;
    vec2 shiftedUV = vec2(uv.x, uv.y + wave * iResolution.y);
    vec4 texColor = texture(samp, shiftedUV / iResolution);
    color = texColor;
}
