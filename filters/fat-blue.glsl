#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec2 center = vec2(0.5, 0.5);
    vec2 dir = tc - center;
    float stretch_factor = 1.0 + 0.5 * abs(sin(time_f * 3.14));
    vec2 new_tc = center + dir;

    vec4 original_color = texture(samp, tc);
    vec4 stretched_color = texture(samp, center + vec2(dir.x / stretch_factor, dir.y));

    color = vec4(original_color.r, original_color.g, stretched_color.b, original_color.a);
}
