#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float random(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(mix(random(i + vec2(0.0, 0.0)), random(i + vec2(1.0, 0.0)), u.x),
               mix(random(i + vec2(0.0, 1.0)), random(i + vec2(1.1, 1.0)), u.x), u.y);
}

float bubble(vec2 uv, vec2 position, float size, float time) {
    float dist = length(uv - position);
    float pop = smoothstep(size, size - 0.05, dist) * (1.0 - smoothstep(size - 0.05, size - 0.1, dist));
    return pop * (1.0 - smoothstep(0.0, 1.0, time));
}

void main(void) {
    vec2 uv = tc;
    float time = mod(time_f, 10.0);

    float boil = noise(uv * 10.0 + time * 2.0) * 0.1;
    boil += noise(uv * 15.0 + time * 3.0) * 0.05;
    boil += noise(uv * 20.0 + time * 5.0) * 0.02;

    vec2 distorted_uv = uv + boil * vec2(noise(uv + time), noise(uv + time + vec2(10.0, 10.0)));

    vec3 col = texture(samp, distorted_uv).rgb;

    float bubble1 = bubble(uv, vec2(0.5, 0.5), 0.1, time);
    float bubble2 = bubble(uv, vec2(0.3, 0.6), 0.08, time + 2.0);
    float bubble3 = bubble(uv, vec2(0.7, 0.4), 0.12, time + 4.0);

    col += vec3(bubble1 + bubble2 + bubble3) * 0.5;
    color = vec4(col, 1.0);
}
