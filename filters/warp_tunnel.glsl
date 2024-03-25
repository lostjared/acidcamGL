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


void main() {
    vec2 uv = (tc - 0.5) * (iResolution.x / iResolution.y, 1.0);
    float radius = length(uv);
    float warp = radius * 10.0 - time_f * 5.0;
    float angle = atan(uv.y, uv.x) + time_f;
    vec3 colorMix = 0.5 + 0.5 * cos(warp + vec3(0, 2, 4));
    float vignette = 0.3 / radius;
    color = vec4(colorMix * vignette, 1.0);
    color = xor_RGB(color, texture(samp, tc));
}
