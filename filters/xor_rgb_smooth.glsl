#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

vec4 xor_RGB(vec4 icolor, vec4 source) {
    ivec3 int_color;
    ivec4 isource = ivec4(source * 255);
    for(int i = 0; i < 3; ++i) {
        int_color[i] = int(255 * icolor[i]);
        int_color[i] = int_color[i]^isource[i];
        if(int_color[i] > 255)
            int_color[i] = int_color[i]%255;
        icolor[i] = float(int_color[i])/255;
    }
    return icolor;
}

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

vec4 blur(sampler2D image, vec2 uv, vec2 resolution) {
    vec2 texelSize = 1.0 / resolution;
    vec4 result = vec4(0.0);
    float kernel[49];
    kernel[0] = 0.5; kernel[1] = 1.0; kernel[2] = 2.0; kernel[3] = 2.5; kernel[4] = 2.0; kernel[5] = 1.0; kernel[6] = 0.5;
    kernel[7] = 1.0; kernel[8] = 2.0; kernel[9] = 3.0; kernel[10] = 3.5; kernel[11] = 3.0; kernel[12] = 2.0; kernel[13] = 1.0;
    kernel[14] = 2.0; kernel[15] = 3.0; kernel[16] = 4.0; kernel[17] = 4.5; kernel[18] = 4.0; kernel[19] = 3.0; kernel[20] = 2.0;
    kernel[21] = 2.5; kernel[22] = 3.5; kernel[23] = 4.5; kernel[24] = 5.0; kernel[25] = 4.5; kernel[26] = 3.5; kernel[27] = 2.5;
    kernel[28] = 2.0; kernel[29] = 3.0; kernel[30] = 4.0; kernel[31] = 4.5; kernel[32] = 4.0; kernel[33] = 3.0; kernel[34] = 2.0;
    kernel[35] = 1.0; kernel[36] = 2.0; kernel[37] = 3.0; kernel[38] = 3.5; kernel[39] = 3.0; kernel[40] = 2.0; kernel[41] = 1.0;
    kernel[42] = 0.5; kernel[43] = 1.0; kernel[44] = 2.0; kernel[45] = 2.5; kernel[46] = 2.0; kernel[47] = 1.0; kernel[48] = 0.5;

    float kernelSum = 272.0;

    for (int x = -3; x <= 3; ++x) {
        for (int y = -3; y <= 3; ++y) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(image, uv + offset) * kernel[(y + 3) * 7 + (x + 3)];
        }
    }

    return result / kernelSum;
}

void main(void) {
    vec4 tcolor = blur(samp, tc, iResolution);
    float time_t = pingPong(time_f, 10.0) + 1.0;
    color = xor_RGB(tcolor, tcolor * time_t);
}
