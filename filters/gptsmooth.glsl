#version 330 core

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

vec4 xor_RGB(vec4 icolor, vec4 source) {
    ivec3 int_color = ivec3(icolor.rgb * 255.0);
    ivec3 isource = ivec3(source.rgb * 255.0);
    for(int i = 0; i < 3; ++i) {
        int_color[i] = int_color[i] ^ isource[i];
        int_color[i] = int_color[i] % 256; // Ensure value stays within 0-255
        icolor[i] = float(int_color[i]) / 255.0;
    }
    return icolor;
}

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

vec4 enhancedBlur(sampler2D image, vec2 uv, vec2 resolution) {
    vec2 texelSize = 1.0 / resolution;
    vec4 result = vec4(0.0);
    float totalWeight = 0.0;

    // Adjust the blur radius for a stronger effect
    int radius = 5;
    for (int x = -radius; x <= radius; ++x) {
        for (int y = -radius; y <= radius; ++y) {
            float distance = length(vec2(x, y));
            float weight = exp(-distance * distance / (2.0 * radius * radius));
            vec2 offset = vec2(x, y) * texelSize;
            result += texture(image, uv + offset) * weight;
            totalWeight += weight;
        }
    }
    return result / totalWeight;
}

void main(void) {
    vec2 uv = tc;

    // Introduce a subtle wave effect for more dynamism
    uv.x += sin(uv.y * 20.0 + time_f * 2.0) * 0.005;
    uv.y += cos(uv.x * 20.0 + time_f * 2.0) * 0.005;

    vec4 tcolor = enhancedBlur(samp, uv, iResolution);

    // Modify time variable for smoother transitions
    float time_t = pingPong(time_f * 0.3, 5.0) + 1.0;

    // Create a dynamic color modulation using time and position
    vec4 modColor = vec4(
        tcolor.r * (0.5 + 0.5 * sin(time_f + uv.x * 10.0)),
        tcolor.g * (0.5 + 0.5 * sin(time_f + uv.y * 10.0)),
        tcolor.b * (0.5 + 0.5 * sin(time_f + (uv.x + uv.y) * 5.0)),
        tcolor.a
    );

    color = xor_RGB(sin(tcolor * time_f), modColor);

    // Enhance contrast for a more pronounced effect
    color.rgb = pow(color.rgb, vec3(1.2));
}
