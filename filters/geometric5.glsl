#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

void main(void) {
    vec2 uv = tc;
    vec2 center = vec2(0.5, 0.5) * iResolution / vec2(iResolution.y);
    vec2 pos = uv - center;
    pos *= 2.0;
    float angle = time_f;
    float cosA = cos(angle);
    float sinA = sin(angle);
    mat2 rot = mat2(cosA, -sinA, sinA, cosA);
    pos = rot * pos;
    float depth = mod(pos.x + pos.y, 0.5) - 0.25;
    float height = sin(depth * 3.14159) * 0.25; // Sin wave for a wavy effect
    vec3 baseColor = vec3(0.5 + 0.5 * sin(time_f), 0.5 + 0.5 * sin(time_f + 2.0), 0.5 + 0.5 * sin(time_f + 4.0));
    float light = 0.5 + 0.5 * sin(depth * 3.14159 + time_f);
    vec3 color3D = height * light * baseColor;
    color3D = sin(color3D * time_f);
    vec4 texColor = texture(samp, uv);
    color = mix(texColor, vec4(color3D, 1.0), 0.3);
}
