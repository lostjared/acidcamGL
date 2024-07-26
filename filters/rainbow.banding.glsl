#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

vec3 rainbowBanding(vec2 uv) {
    float banding = sin(uv.y * 200.0 + time_f * 5.0);
    vec3 col;
    col.r = sin(banding + 0.0) * 0.05 + 0.95;
    col.g = sin(banding + 2.0) * 0.05 + 0.95;
    col.b = sin(banding + 4.0) * 0.05 + 0.95;
    return col;
}

vec3 compositeEffect(vec2 uv) {
    vec3 col = texture(samp, uv).rgb;
    vec3 banding = rainbowBanding(uv);
    col *= banding;
    float noise = fract(sin(dot(uv.xy, vec2(12.9898, 78.233))) * 43758.5453);
    col += noise * 0.02;
    float scanline = sin(uv.y * iResolution.y * 3.0) * 0.1;
    col -= scanline;
    return col;
}

void main(void) {
    vec2 uv = tc;
    vec3 col = compositeEffect(uv);
    color = vec4(col, 1.0);
}
