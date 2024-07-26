#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

vec3 analogEffect(vec2 uv) {
    vec3 col = texture(samp, uv).rgb;

    float offset = 0.002;
    col.r = texture(samp, uv + vec2(offset, 0.0)).r;
    col.b = texture(samp, uv - vec2(offset, 0.0)).b;

    float noise = fract(sin(dot(uv.xy + time_f, vec2(12.9898, 78.233))) * 43758.5453);
    col += noise * 0.05;

    float scanline = sin(uv.y * iResolution.y * 3.0 + time_f * 20.0) * 0.05;
    col -= vec3(scanline);

    float vignette = smoothstep(1.0, 0.8, length(uv - 0.5) * 1.5);
    col *= vignette;

    return col;
}

void main(void) {
    vec2 uv = tc;
    vec3 col = analogEffect(uv);
    color = vec4(col, 1.0);
}
