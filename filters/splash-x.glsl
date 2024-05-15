#version 330 core

in vec2 tc;
out vec4 color;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void)
{
    vec2 uv = tc;
    vec2 center = vec2(0.5, 0.5);
    float distance = length(uv - center);

    float ripple = sin(10.0 * distance - time_f * 3.0);
    ripple *= sin(time_f) / distance;

    uv += (uv - center) * ripple;

    color = texture(samp, uv);
    color = mix(color, vec4(0.0, 0.5, 1.0, 1.0), 0.2);
}
