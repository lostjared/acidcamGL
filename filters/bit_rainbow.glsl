#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float NESQuantize(float val) {
    return floor(val * 16.0) / 15.0;
}

vec3 NESPalette(vec3 col) {
    col.r = NESQuantize(col.r);
    col.g = NESQuantize(col.g);
    col.b = NESQuantize(col.b);
    return col;
}

vec3 rainbowGradient(float t) {
    float r = 0.5 + 0.5 * cos(6.28318 * t + 0.0);
    float g = 0.5 + 0.5 * cos(6.28318 * t + 2.09439);
    float b = 0.5 + 0.5 * cos(6.28318 * t + 4.18878);
    return vec3(r, g, b);
}

void main(void) {
    vec2 uv = tc * iResolution.xy / min(iResolution.x, iResolution.y);
    vec2 center = vec2(0.5, 0.5);
    float angle = atan(uv.y - center.y, uv.x - center.x) + time_f;
    float radius = length(uv - center);

    vec3 gradientColor = rainbowGradient(angle / (2.0 * 3.14159));
    gradientColor *= smoothstep(0.4, 0.6, radius);

    vec4 texColor = texture(samp, tc);
    vec3 blendedColor = mix(texColor.rgb, gradientColor, 0.5);

    vec3 nesColor = NESPalette(blendedColor);

    color = vec4(nesColor, texColor.a);
}
