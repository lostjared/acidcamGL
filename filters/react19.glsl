#version 330 core
out vec4 color;
in vec2 tc;

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
    float kernel[100];
    
    float kernelVals[100] = float[](0.5, 1.0, 1.5, 2.0, 2.5, 2.5, 2.0, 1.5, 1.0, 0.5,
                                    1.0, 2.0, 2.5, 3.0, 3.5, 3.5, 3.0, 2.5, 2.0, 1.0,
                                    1.5, 2.5, 3.0, 3.5, 4.0, 4.0, 3.5, 3.0, 2.5, 1.5,
                                    2.0, 3.0, 3.5, 4.0, 4.5, 4.5, 4.0, 3.5, 3.0, 2.0,
                                    2.5, 3.5, 4.0, 4.5, 5.0, 5.0, 4.5, 4.0, 3.5, 2.5,
                                    2.5, 3.5, 4.0, 4.5, 5.0, 5.0, 4.5, 4.0, 3.5, 2.5,
                                    2.0, 3.0, 3.5, 4.0, 4.5, 4.5, 4.0, 3.5, 3.0, 2.0,
                                    1.5, 2.5, 3.0, 3.5, 4.0, 4.0, 3.5, 3.0, 2.5, 1.5,
                                    1.0, 2.0, 2.5, 3.0, 3.5, 3.5, 3.0, 2.5, 2.0, 1.0,
                                    0.5, 1.0, 1.5, 2.0, 2.5, 2.5, 2.0, 1.5, 1.0, 0.5);
    
    for (int i = 0; i < 100; i++) {
        kernel[i] = kernelVals[i];
    }
    float kernelSum = 842.0;

    for (int x = -5; x <= 4; ++x) {
        for (int y = -5; y <= 4; ++y) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(image, uv + offset) * kernel[(y + 5) * 10 + (x + 5)];
        }
    }

    return result / kernelSum;
}

void main() {
    vec2 uv = tc / iResolution;
    float amplitude = sin(time_f) * 0.5 + 0.5;
    vec3 gradientColor = mix(vec3(0.0, 0.0, 1.0), vec3(1.0, 0.0, 0.0), amplitude);
    
    vec4 texColor = blur(samp, tc, iResolution);
    vec3 finalColor = mix(texColor.rgb, gradientColor, 0.5 * sin(time_f * 2.0 + uv.y * 3.14159) + 0.5);
    
    color = vec4(finalColor, texColor.a);
    float time_t = pingPong(time_f, 10.0);
    color = xor_RGB(color, blur(samp, tc, iResolution) * time_t);

    if(color.rgb == vec3(color.r)) {
        color.rgb += vec3(0.05, 0.05, 0.05) * sin(time_f * 3.0);
    }
}
