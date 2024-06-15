#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform sampler2D mat_samp;
uniform float time_f;
uniform vec2 iResolution;

float hash(float n) {
    return fract(sin(n) * 43758.5453123);
}

vec2 random2(vec2 st) {
    st = vec2(dot(st, vec2(127.1, 311.7)),
              dot(st, vec2(269.5, 183.3)));
    return -1.0 + 2.0 * fract(sin(st) * 43758.5453123);
}

vec2 smoothRandom2(float t) {
    float t0 = floor(t);
    float t1 = t0 + 1.0;
    vec2 rand0 = random2(vec2(t0));
    vec2 rand1 = random2(vec2(t1));
    float mix_factor = fract(t);
    return mix(rand0, rand1, smoothstep(0.0, 1.0, mix_factor));
}

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
    float wave = sin(uv.x * 10.0 + time_f * 2.0) * 0.1;
    vec2 random_direction = smoothRandom2(time_f) * 0.5;
    float expand = 0.5 + 0.5 * sin(time_f * 2.0);
    vec2 spiral_uv = uv * expand + random_direction;
    float angle = atan(spiral_uv.y + wave, spiral_uv.x) + time_f * 2.0;
    vec3 rainbow_color = rainbow(angle / (2.0 * 3.14159));
    vec4 color1 = texture(samp, tc);
    vec4 color2 = texture(mat_samp, tc);
    float blend_factor = (sin(time_f) + 1.0) * 0.5;
    vec4 blended_color = mix(color1, color2, blend_factor);
    vec3 final_color = mix(blended_color.rgb, rainbow_color, 0.5);
    float time_t = mod(time_f, 30.0);
    color = vec4(sin(final_color * time_t), blended_color.a);
}
