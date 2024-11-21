#version 330 core

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;
void main() {
    vec2 uv = tc;
    vec2 center = vec2(0.5, 0.5);
    vec2 offset = uv - center;

    float ripple = sin(length(offset) * 20.0 - time_f * 5.0) * 0.05;
    float angle = atan(offset.y, offset.x) + ripple * sin(time_f);
    float radius = length(offset);
    vec2 swirlUV = center + radius * vec2(cos(angle), sin(angle));
    float pulse = 0.2 * sin(time_f * 3.0);
    swirlUV += pulse * normalize(offset);
    vec4 texColor = texture(samp, swirlUV);
    vec3 shiftedColor = texColor.rgb * (0.8 + 0.2 * sin(time_f * 2.0));
    color = vec4(shiftedColor, texColor.a);
}
