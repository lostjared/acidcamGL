#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;
uniform float alpha_r; // Random value for red channel
uniform float alpha_g; // Random value for green channel
uniform float alpha_b; // Random value for blue channel

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

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}


void main(void) {
    vec3 colorValue = blur(samp, tc, iResolution).rgb;

    float time_t = pingPong(time_f, 20.0) + 2.0;
    
    float maxColorValue = 1.0;
    float timeCycle = mod(time_f, 6.0);

    if (timeCycle < 1.0) {
        colorValue.r = mix(0.0, maxColorValue, timeCycle);
    } else if (timeCycle < 2.0) {
        colorValue.r = maxColorValue;
        colorValue.g = mix(0.0, maxColorValue, timeCycle - 1.0);
    } else if (timeCycle < 3.0) {
        colorValue.r = maxColorValue;
        colorValue.g = maxColorValue;
        colorValue.b = mix(0.0, maxColorValue, timeCycle - 2.0);
    } else if (timeCycle < 4.0) {
        colorValue.r = maxColorValue;
        colorValue.g = maxColorValue;
        colorValue.b = maxColorValue;
        colorValue.b = mix(maxColorValue, alpha_b, timeCycle - 3.0);
    } else if (timeCycle < 5.0) {
        colorValue.r = maxColorValue;
        colorValue.g = maxColorValue;
        colorValue.b = alpha_b;
        colorValue.g = mix(maxColorValue, alpha_g, timeCycle - 4.0);
    } else {
        colorValue.r = maxColorValue;
        colorValue.g = alpha_g;
        colorValue.b = alpha_b;
        colorValue.r = mix(maxColorValue, alpha_r, timeCycle - 5.0);
    }

    color = vec4(colorValue, 1.0);
    color = xor_RGB(blur(samp, tc, iResolution), color);
    color = sin(color * time_t);
}
