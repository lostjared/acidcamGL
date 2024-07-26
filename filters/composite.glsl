#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

vec3 compositeEffect(vec2 uv) {
    float offset = 0.01;
    vec3 col;
    col.r = texture(samp, uv + vec2(offset, 0.0)).r;
    col.g = texture(samp, uv).g;
    col.b = texture(samp, uv - vec2(offset, 0.0)).b;
    float noise = fract(sin(dot(uv.xy, vec2(12.9898, 78.233))) * 43758.5453);
    col += noise * 0.05;
    float scanline = sin(uv.y * iResolution.y * 1.5) * 0.1;
    col -= scanline;
       float bleed = sin(uv.y * iResolution.y * 0.2 + time_f * 5.0) * 0.005;
    col.r += bleed * 0.002;
    col.b -= bleed * 0.002;
    return col;
}

void main(void) {
    vec2 uv = tc;
    vec3 col = compositeEffect(uv);
    color = vec4(col, 1.0);
}
