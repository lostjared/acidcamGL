#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float noise(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(void) {
    vec2 uv = tc;
    float n = noise(uv * time_f);
    float offset = sin(uv.y * 10.0 + time_f) * 0.1;
    uv.x += offset * n;
    vec4 texColor = texture(samp, uv);
    float stripe = step(0.5, fract(uv.y * 80.0 + time_f));
    texColor.rgb *= stripe;
    texColor.rgb += n * 0.2;
    color = 1.0-texColor;
}
