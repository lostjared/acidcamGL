#version 330 core

in vec2 tc;
out vec4 color;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;
uniform float restore_black;
in float restore_black_value;
uniform vec4 inc_valuex;

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

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

void main(void) {
    if (restore_black_value == 1.0 && texture(samp, tc) == vec4(0, 0, 0, 1))
        discard;

    vec2 uv = tc * 2.0 - 1.0;
    uv.y *= iResolution.y / iResolution.x;
    
    float time_t = pingPong(time_f, 15.0) + 1.0;
    float wave = sin(uv.x * 10.0 + time_t * 2.0) * 0.1;
    vec2 random_direction = smoothRandom2(time_t) * 0.5;
    float expand = 0.5 + 0.5 * sin(time_t * 2.0);
    vec2 spiral_uv = uv * expand + random_direction;
    float angle = atan(spiral_uv.y + wave, spiral_uv.x) + time_t * 2.0;
    vec3 rainbow_color = rainbow(angle / (2.0 * 3.14159));
    vec4 original_color = texture(samp, tc);
    vec3 blended_color = mix(original_color.rgb, rainbow_color, 0.5);

    color = vec4(sin(blended_color * time_t), original_color.a);

    vec2 pos = gl_FragCoord.xy / iResolution;
    vec4 s = color * sin(inc_valuex / 255.0 * time_f);
    color[0] += s[0] * pos[0];
    color[1] += s[1] * pos[1];

    time_t = pingPong(time_f, 20) + 2.0;
    color = sin(color * time_t);
}
