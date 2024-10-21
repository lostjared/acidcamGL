#version 330 core
in vec2 tc;
out vec4 color;

uniform sampler2D samp;
uniform float time_f;

void main(void)
{
    vec2 center = vec2(0.5, 0.5);
    float angle = time_f;
    vec2 tc1 = tc;
    vec2 tc2 = tc - center;
    tc2 *= 0.8;
    tc2 = vec2(
        tc2.x * cos(angle + 1.0) - tc2.y * sin(angle + 1.0),
        tc2.x * sin(angle + 1.0) + tc2.y * cos(angle + 1.0)
    );
    tc2 += center;
    tc2 = fract(tc2);
    vec2 tc3 = tc - center;
    tc3 *= 0.6;
    tc3 = vec2(
        tc3.x * cos(angle + 2.0) - tc3.y * sin(angle + 2.0),
        tc3.x * sin(angle + 2.0) + tc3.y * cos(angle + 2.0)
    );
    tc3 += center;
    tc3 = fract(tc3);
    vec2 tc4 = tc - center;
    tc4 *= 0.4;
    tc4 = vec2(
        tc4.x * cos(angle + 3.0) - tc4.y * sin(angle + 3.0),
        tc4.x * sin(angle + 3.0) + tc4.y * cos(angle + 3.0)
    );
    tc4 += center;
    tc4 = fract(tc4);
    vec4 color1 = texture(samp, tc1);
    vec4 color2 = texture(samp, tc2);
    vec4 color3 = texture(samp, tc3);
    vec4 color4 = texture(samp, tc4);
    color = (color1 + color2 + color3 + color4) * 0.25;
}

