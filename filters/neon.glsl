#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;
uniform float value_alpha_r, value_alpha_g, value_alpha_b;

vec3 overlayBlend(vec3 base, vec3 blend, float opacity) {
    vec3 c2 = blend * 2.0;
    vec3 c1 = 1.0 - 2.0 * (1.0 - blend);
    vec3 result = mix(base * c2, c1, step(0.5, base));
    return mix(base, result, opacity);
}

void main(void) {
    vec2 uv = tc;
    vec2 center = vec2(0.5, 0.5);
    vec2 normCoord = 2.0 * (uv - center);
    float dist = length(normCoord);
    float angle = atan(normCoord.y, normCoord.x) + time_f * 5.0;
    float spiral = cos(10.0 * dist - angle);
    float mask = smoothstep(0.1, 0.2, abs(spiral) - dist * 0.5);
    vec3 neonPurple = vec3(value_alpha_r, value_alpha_g, value_alpha_b);
    vec3 originalTexture = texture(samp, uv).rgb;
    vec3 blendedColor = overlayBlend(originalTexture, neonPurple, mask);
    color = vec4(blendedColor, 1.0);
}
