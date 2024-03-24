#version 330 core

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;
in float alpha;

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
    vec4 col2 = texture(samp, tc);
    float t = mod(time_f, 10) * mod(alpha, 1.0);
    
    col2 *= t;
    
    vec4 col = xor_RGB(texture(samp, tc), col2);
    color = col;
}
    
