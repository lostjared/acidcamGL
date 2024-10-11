#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

vec3 rainbow(float t) {
    t = fract(t);
    float r = abs(t * 6.0 - 3.0) - 1.0;
    float g = 2.0 - abs(t * 6.0 - 2.0);
    float b = 2.0 - abs(t * 6.0 - 4.0);
    return clamp(vec3(r, g, b), 0.0, 1.0);
}

void main(void) {
    vec2 uv = tc * 2.0 - 1.0;
    uv.y *= iResolution.y / iResolution.x;
    float angle = atan(uv.y, uv.x);
    float radius = length(uv);
    float dispersion = 0.02;
    vec2 dir = normalize(uv);
    vec2 offset_r = tc + dir * dispersion;
    vec2 offset_g = tc;
    vec2 offset_b = tc - dir * dispersion;
    float r = texture(samp, offset_r).r;
    float g = texture(samp, offset_g).g;
    float b = texture(samp, offset_b).b;
    vec3 prism_color = vec3(r, g, b);
    float t = fract((angle / (2.0 * 3.14159)) + time_f * 0.1);
    vec3 rainbow_color = rainbow(t);
    float rainbow_factor = 0.5;
    vec3 final_color = mix(prism_color, rainbow_color, rainbow_factor);
    color = vec4(final_color, 1.0);
}

