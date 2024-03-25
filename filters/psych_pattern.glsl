#version 330 core

in vec2 tc;
out vec4 color;
uniform float time_f;
uniform vec2 iResolution;
uniform sampler2D samp;

vec4 xor_RGB(vec4 icolor, vec4 isourcex) {
    ivec4 isource = ivec4(isourcex * 255);
    ivec3 int_color;
    for(int i = 0; i < 3; ++i) {
        int_color[i] = int(255 * icolor[i]);
        int_color[i] ^= isource[i];
        if(int_color[i] > 255)
            int_color[i] %= 255;
        icolor[i] = float(int_color[i]) / 255.0;
    }
    return icolor;
}


float kaleido(vec2 uv) {
    uv = uv * 2.0 - 1.0;
    float angle = atan(uv.y, uv.x) + time_f;
    float radius = length(uv);
    angle = mod(angle, 3.14159 / 4.0) * 8.0;
    return abs(cos(angle * radius * 10.0 - time_f * 4.0));
}

vec3 colorCycle(float t) {
    return vec3(
        sin(t * 2.0 + 0.0) * 0.5 + 0.5,
        sin(t * 2.0 + 2.09439) * 0.5 + 0.5,
        sin(t * 2.0 + 4.18879) * 0.5 + 0.5
    );
}

void main() {
    vec2 uv = tc - 0.5;
    uv.x *= iResolution.x / iResolution.y;
    float pattern = kaleido(uv);
    vec3 col = colorCycle(pattern + time_f);
    color = vec4(col, 1.0);
    color = xor_RGB(color, texture(samp, tc));
}
