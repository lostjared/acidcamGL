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

    vec4 color1 = texture(samp, tc1);
    vec4 color2 = texture(samp, tc2);
    vec4 color3 = texture(samp, tc3);
    float pi = 3.14159265;
    float offset = 2.0 * pi / 3.0;

    float w0 = sin(time_f) * 0.5 + 0.5;
    float w1 = sin(time_f + offset) * 0.5 + 0.5;
    float w2 = sin(time_f + 2.0 * offset) * 0.5 + 0.5;

    vec3 weights1 = vec3(w0, 0.0, 0.0);
    vec3 weights2 = vec3(0.0, w1, 0.0);
    vec3 weights3 = vec3(0.0, 0.0, w2);

    vec3 finalColor = color1.rgb * weights1 + color2.rgb * weights2 + color3.rgb * weights3;

    color = vec4(finalColor, 1.0);
}

