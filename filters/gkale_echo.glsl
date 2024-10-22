#version 330 core
in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;
float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}
vec4 firstEffect(vec2 tc) {
    vec2 center = vec2(0.5, 0.5);
    float angle = time_f;
    vec2 tc1 = tc;
    vec2 tc2 = tc - center;
    tc2 *= 0.8;
    tc2 = vec2(
        tc2.x * cos(angle + 1.0) - tc2.y * sin(angle + 1.0),
        tc2.x * sin(angle + 1.0) + tc2.y * cos(angle + 1.0)
    );
    tc2 += center;
    tc2 = fract(tc2);
    vec2 tc3 = tc - center;
    tc3 *= 0.6;
    tc3 = vec2(
        tc3.x * cos(angle + 2.0) - tc3.y * sin(angle + 2.0),
        tc3.x * sin(angle + 2.0) + tc3.y * cos(angle + 2.0)
    );
    tc3 += center;
    tc3 = fract(tc3);
    vec2 tc4 = tc - center;
    tc4 *= 0.4;
    tc4 = vec2(
        tc4.x * cos(angle + 3.0) - tc4.y * sin(angle + 3.0),
        tc4.x * sin(angle + 3.0) + tc4.y * cos(angle + 3.0)
    );
    tc4 += center;
    tc4 = fract(tc4);
    vec4 color1 = texture(samp, tc1);
    vec4 color2 = texture(samp, tc2);
    vec4 color3 = texture(samp, tc3);
    vec4 color4 = texture(samp, tc4);
    return (color1 + color2 + color3 + color4) * 0.25;
}
void main(void) {
    vec2 uv = tc * 2.0 - 1.0;
    uv.x *= iResolution.x / iResolution.y;
    float angle = atan(uv.y, uv.x);
    float radius = length(uv) * 1.4142;
    float segments = 8.0;
    angle = mod(angle, 6.28318 / segments);
    angle = abs(angle - 3.14159 / segments);
    uv = vec2(cos(angle), sin(angle)) * radius;
    uv = uv * 0.5 + 0.5;
    uv = clamp(uv, 0.0, 1.0);
    float time_t = pingPong(time_f * 0.5, 7.0) + 2.0;
    vec4 texColor = firstEffect(uv);
    float pattern = cos(radius * 10.0 - time_t * 5.0);
    vec3 colorShift = vec3(
        0.5 + 0.5 * cos(pattern + time_t + 0.0),
        0.5 + 0.5 * cos(pattern + time_t + 2.094),
        0.5 + 0.5 * cos(pattern + time_t + 4.188)
    );
    vec3 finalColor = texColor.rgb * colorShift;
    color = vec4(finalColor, texColor.a);
    color = mix(color, firstEffect(tc), 0.5);
    color = sin(color * time_t);
}

