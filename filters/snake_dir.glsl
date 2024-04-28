
#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;
in float alpha;

vec4 snake1() {
    vec2 uv = tc * iResolution;
    float wave = sin(uv.x * 0.05 + time_f * 2.0) * 0.05;
    vec2 shiftedUV = vec2(uv.x, uv.y + wave * iResolution.y);
    vec4 texColor = texture(samp, shiftedUV / iResolution);
    return texColor;
}

vec4 snake2() {
    vec2 uv = tc * iResolution;
    float wave = sin(uv.y * 0.05 + time_f * 2.0) * 0.05;
    vec2 shiftedUV = vec2(uv.x + wave * iResolution.x, uv.y);
    vec4 texColor = texture(samp, shiftedUV / iResolution);
    return texColor;
}

void main(void) {
    float time_t = mod(time_f, 10);
    color = sin(mix(snake1(), snake2(), 0.5) * (alpha * time_t));
}
