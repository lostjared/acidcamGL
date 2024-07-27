#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float random(float x) {
    return fract(sin(x) * 43758.5453123);
}

void main(void) {
    vec2 uv = tc;
    float y = uv.y * iResolution.y;
    float offset = sin(y * 10.0 + time_f * 5.0) * 0.005;

    if (random(y + time_f * 10.0) < 0.1) {
        offset += (random(y) - 0.5) * 0.02;
    }

    uv.x += offset;
    vec4 texColor = texture(samp, uv);

    float r = texture(samp, uv + vec2(0.002, 0.0)).r;
    float g = texture(samp, uv).g;
    float b = texture(samp, uv - vec2(0.002, 0.0)).b;

    vec4 compositeColor = vec4(r, g, b, texColor.a);

    vec4 vhsColor = mix(texColor, compositeColor, 0.5);

    vhsColor.rgb = vhsColor.rgb * vec3(0.9, 0.85, 0.8) + vec3(0.1, 0.1, 0.15);
    vhsColor.rgb = clamp(vhsColor.rgb, 0.0, 1.0);

    color = vhsColor;
}
