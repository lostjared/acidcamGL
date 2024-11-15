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
    float hue = mod(t, 1.0) * 6.0;
    float c = 1.0;
    float x = 1.0 - abs(mod(hue, 2.0) - 1.0);
    vec3 rgb = hue < 1.0 ? vec3(c, x, 0.0) :
               hue < 2.0 ? vec3(x, c, 0.0) :
               hue < 3.0 ? vec3(0.0, c, x) :
               hue < 4.0 ? vec3(0.0, x, c) :
               hue < 5.0 ? vec3(x, 0.0, c) :
                           vec3(c, 0.0, x);
    return rgb;
}

void main(void) {
    vec2 center = vec2(0.5, 0.5);
    vec2 dir = tc - center;
    float dist = length(dir);
    float angle = atan(dir.y, dir.x);
    float rippleFrequency = 10.0;
    float rippleAmplitude = 0.02;
    float ripple = sin(dist * rippleFrequency - time_f * 5.0) * rippleAmplitude;
    vec2 ripple_tc = tc + normalize(dir) * ripple;
    ripple_tc = clamp(ripple_tc, vec2(0.0), vec2(1.0));
    float gradient_pos = mod(dist * 3.0 + angle / (2.0 * 3.14159) + time_f * 0.5, 1.0);
    vec3 color_gradient = rainbow(gradient_pos);
    vec4 ctx = texture(samp, ripple_tc);
    float time_t = pingPong(time_f, 10.0) + 2.0;
    color = mix(sin(ctx * time_t), vec4(color_gradient, 1.0), 0.5);
}

