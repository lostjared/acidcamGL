#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

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

    float kernelSum = 0.0;
    for (int i = 0; i < 100; i++) {
        kernelSum += kernel[i];
    }

    for (int x = -5; x <= 4; ++x) {
        for (int y = -5; y <= 4; ++y) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(image, uv + offset) * kernel[(y + 5) * 10 + (x + 5)];
        }
    }

    return result / kernelSum;
}

vec4 colorShift(vec4 col) {
    return vec4(
        0.5 + 0.5 * cos(col.r * 3.14159265 * 0.5),
        0.5 + 0.5 * cos(col.g * 3.14159265 * 0.5),
        0.5 + 0.5 * cos(col.b * 3.14159265 * 0.5),
        col.a
    );
}

void main(void) {
    float time_t = pingPong(time_f, 10.0) + 1.0;
    vec4 pix = blur(samp, tc, iResolution);
    pix = pix * time_t;
    pix = colorShift(pix);
    pix.rgb = mix(vec3(1.0), pix.rgb, 0.8);
    color = pix;
}
