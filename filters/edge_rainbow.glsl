#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

vec3 edgeDetection(sampler2D tex, vec2 uv) {
    vec2 offset = vec2(1.0 / iResolution.x, 1.0 / iResolution.y);

    float gx =
          -1.0 * texture(tex, uv + vec2(-offset.x, -offset.y)).r
        + -2.0 * texture(tex, uv + vec2(-offset.x, 0.0)).r
        + -1.0 * texture(tex, uv + vec2(-offset.x, offset.y)).r
        +  1.0 * texture(tex, uv + vec2(offset.x, -offset.y)).r
        +  2.0 * texture(tex, uv + vec2(offset.x, 0.0)).r
        +  1.0 * texture(tex, uv + vec2(offset.x, offset.y)).r;

    float gy =
          -1.0 * texture(tex, uv + vec2(-offset.x, -offset.y)).r
        + -2.0 * texture(tex, uv + vec2(0.0, -offset.y)).r
        + -1.0 * texture(tex, uv + vec2(offset.x, -offset.y)).r
        +  1.0 * texture(tex, uv + vec2(-offset.x, offset.y)).r
        +  2.0 * texture(tex, uv + vec2(0.0, offset.y)).r
        +  1.0 * texture(tex, uv + vec2(offset.x, offset.y)).r;

    float edge = sqrt(gx * gx + gy * gy);
    float threshold = 0.3;
    vec3 edgeColor = vec3(0.0);

    if (edge > threshold) {
        float hue = mod(time_f + length(uv), 1.0);
        float r = abs(hue * 6.0 - 3.0) - 1.0;
        float g = 2.0 - abs(hue * 6.0 - 2.0);
        float b = 2.0 - abs(hue * 6.0 - 4.0);
        edgeColor = clamp(vec3(r, g, b), 0.0, 1.0);
    }

    return edgeColor;
}

void main(void) {
    vec3 edgeColor = edgeDetection(samp, tc);
    color = vec4(edgeColor, 1.0);
}
