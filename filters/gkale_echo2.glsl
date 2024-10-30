#version 330 core

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

vec4 firstEffect(vec2 tc) {
    vec2 center = vec2(0.5, 0.5);
    float angle = time_f;

    vec2 tc1 = tc;
    vec2 tc2 = (tc - center) * 0.9;
    tc2 = vec2(
        tc2.x * cos(angle + 1.5) - tc2.y * sin(angle + 1.5),
        tc2.x * sin(angle + 1.5) + tc2.y * cos(angle + 1.5)
    ) + center;
    tc2 = fract(tc2);

    vec2 tc3 = (tc - center) * 0.7;
    tc3 = vec2(
        tc3.x * cos(angle + 3.0) - tc3.y * sin(angle + 3.0),
        tc3.x * sin(angle + 3.0) + tc3.y * cos(angle + 3.0)
    ) + center;
    tc3 = fract(tc3);

    vec2 tc4 = (tc - center) * 0.5;
    tc4 = vec2(
        tc4.x * cos(angle + 4.5) - tc4.y * sin(angle + 4.5),
        tc4.x * sin(angle + 4.5) + tc4.y * cos(angle + 4.5)
    ) + center;
    tc4 = fract(tc4);

    vec4 color1 = texture(samp, tc1);
    vec4 color2 = texture(samp, tc2);
    vec4 color3 = texture(samp, tc3);
    vec4 color4 = texture(samp, tc4);

    return (color1 + color2 + color3 + color4) * 0.4;
}

vec4 xor_RGB(vec4 icolor, vec4 source) {
    ivec3 int_color;
    ivec4 isource = ivec4(source * 255.0);
    for (int i = 0; i < 3; ++i) {
        int_color[i] = int(255.0 * icolor[i]);
        int_color[i] = int_color[i] ^ isource[i];
        if (int_color[i] > 255)
            int_color[i] = int_color[i] % 255;
        icolor[i] = float(int_color[i]) / 255.0;
    }
    return icolor;
}

vec4 blur(sampler2D image, vec2 uv, vec2 resolution) {
    vec2 texelSize = 1.0 / resolution;
    vec4 result = vec4(0.0);

    float kernel[100];
    float kernelVals[100] = float[](
        0.5, 1.0, 1.5, 2.0, 2.5, 2.5, 2.0, 1.5, 1.0, 0.5,
        1.0, 2.0, 2.5, 3.0, 3.5, 3.5, 3.0, 2.5, 2.0, 1.0,
        1.5, 2.5, 3.0, 3.5, 4.0, 4.0, 3.5, 3.0, 2.5, 1.5,
        2.0, 3.0, 3.5, 4.0, 4.5, 4.5, 4.0, 3.5, 3.0, 2.0,
        2.5, 3.5, 4.0, 4.5, 5.0, 5.0, 4.5, 4.0, 3.5, 2.5,
        2.5, 3.5, 4.0, 4.5, 5.0, 5.0, 4.5, 4.0, 3.5, 2.5,
        2.0, 3.0, 3.5, 4.0, 4.5, 4.5, 4.0, 3.5, 3.0, 2.0,
        1.5, 2.5, 3.0, 3.5, 4.0, 4.0, 3.5, 3.0, 2.5, 1.5,
        1.0, 2.0, 2.5, 3.0, 3.5, 3.5, 3.0, 2.5, 2.0, 1.0,
        0.5, 1.0, 1.5, 2.0, 2.5, 2.5, 2.0, 1.5, 1.0, 0.5
    );

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

void main(void) {
    // First Effect
    vec2 uv = tc * 2.0 - 1.0;
    uv.x *= iResolution.x / iResolution.y;
    float angle = atan(uv.y, uv.x);
    float radius = length(uv) * 1.4142;
    float segments = 12.0;
    angle = mod(angle, 6.28318 / segments);
    angle = abs(angle - 3.14159 / segments);
    uv = vec2(cos(angle), sin(angle)) * radius;
    uv = uv * 0.5 + 0.5;
    uv = clamp(uv, 0.0, 1.0);

    float time_t1 = pingPong(time_f * 0.7, 7.0) + 2.0;
    vec4 texColor = firstEffect(uv);

    float pattern = sin(radius * 15.0 - time_t1 * 7.0);

    vec3 colorShift = vec3(
        0.6 + 0.4 * cos(pattern + time_t1 * 1.5 + 0.0),
        0.6 + 0.4 * cos(pattern + time_t1 * 1.5 + 2.094),
        0.6 + 0.4 * cos(pattern + time_t1 * 1.5 + 4.188)
    );

    vec3 vibrantColor = texColor.rgb * colorShift * 1.5;
    vibrantColor = clamp(vibrantColor, 0.0, 1.0);
    float time_z = pingPong(time_f, 8.0) + 2.0;

    vec4 color1 = vec4(vibrantColor, texColor.a);
    color1 = mix(sin(color1 * time_z), firstEffect(tc), 0.6);

    vec4 tcolor = blur(samp, uv, iResolution);
    float time_t2 = pingPong(time_f, 10.0) + 2.0;
    color = xor_RGB(color1, tcolor * time_t2);
}

