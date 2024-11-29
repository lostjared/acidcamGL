#version 330 core

uniform float time_f;
uniform vec2 iResolution;
uniform sampler2D samp;
in vec2 tc;
out vec4 color;

vec4 xor_RGB(vec4 icolor, vec4 source) {
    ivec3 int_color;
    ivec4 isource = ivec4(source * 255);
    for(int i = 0; i < 3; ++i) {
        int_color[i] = int(255 * icolor[i]);
        int_color[i] = int_color[i] ^ isource[i];
        if(int_color[i] > 255)
            int_color[i] = int_color[i] % 255;
        icolor[i] = float(int_color[i]) / 255;
    }
    return icolor;
}

void main() {
    vec4 texColor = texture(samp, tc);
    float fluctuation = sin(time_f * 2.0) * 0.5 + 0.5;
    vec3 extremeColor1 = vec3(1.0, 0.0, 0.0);
    vec3 extremeColor2 = vec3(0.0, 0.0, 1.0);
    vec3 fluctuatedColor = mix(extremeColor1, extremeColor2, fluctuation);
    vec4 fluctuatedVec = vec4(fluctuatedColor, 1.0);
    
    color = xor_RGB(texColor, fluctuatedVec);
}
