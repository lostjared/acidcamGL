#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;
uniform float random_seed;

vec3 rainbow(float t) {
    t = fract(t);
    float r = abs(t * 6.0 - 3.0) - 1.0;
    float g = 2.0 - abs(t * 6.0 - 2.0);
    float b = 2.0 - abs(t * 6.0 - 4.0);
    return clamp(vec3(r, g, b), 0.0, 1.0);
}

float noise(vec2 p) {
    return texture(samp, p).r;
}

vec2 swirl(vec2 p, float t, float seed) {
    float radius = length(p);
    float angle = atan(p.y, p.x) + sin(t + radius * 10.0 + noise(p * 10.0 + seed) * 10.0) * 0.5;
    return vec2(cos(angle), sin(angle)) * radius;
}

void main(void) {
    float mod_time = mod(time_f, 19.0);
    vec2 uv = tc * 2.0 - 1.0;
    uv.y *= iResolution.y / iResolution.x;

    uv = swirl(uv, mod_time * 0.1, random_seed);
    uv *= 0.5;

    float n = noise(uv * 0.5 + mod_time * 0.05);
    vec3 rainbow_color = rainbow(n + mod_time * 0.1);

    vec4 original_color = texture(samp, tc);
    vec3 blended_color = mix(original_color.rgb, rainbow_color, 0.5);

    color = vec4(blended_color, original_color.a);
}
