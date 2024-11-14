#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

vec3 rainbow(float t) {
    float r = sin(6.28318 * t + 0.0);
    float g = sin(6.28318 * t + 2.09439);
    float b = sin(6.28318 * t + 4.18878);
    return vec3(r, g, b) * 0.5 + 0.5;
}

void main(void) {
    float swirl_strength = 0.1;
    float swirl_speed = 1.0;
    float angle = swirl_speed * time_f + length(tc - 0.5);
    vec2 swirl_offset = vec2(cos(angle), sin(angle)) * swirl_strength;
    vec2 swirl_tc = tc + swirl_offset;
    swirl_tc = clamp(swirl_tc, vec2(0.0), vec2(1.0));
    float scale = 5.0;
    float speed = 0.2;
    float gradient_pos = 0.5 + 0.5 * sin((swirl_tc.x * scale + swirl_tc.y * scale) + time_f * speed);
    vec3 color_gradient = rainbow(gradient_pos);
    vec4 ctx = texture(samp, swirl_tc);
    color = vec4(sin(ctx.rgb * time_f), ctx.a);
    color = mix(color, vec4(color_gradient, 1.0), 0.5);
}
