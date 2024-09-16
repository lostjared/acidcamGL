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

vec3 hue2rgb(float h) {
    h = mod(h, 1.0);
    float r = abs(h * 6.0 - 3.0) - 1.0;
    float g = 2.0 - abs(h * 6.0 - 2.0);
    float b = 2.0 - abs(h * 6.0 - 4.0);
    return clamp(vec3(r, g, b), 0.0, 1.0);
}

void main(void) {
    vec2 uv = tc;
    vec2 center = vec2(0.5, 0.5);
    vec2 dir = uv - center;
    float dist = length(dir);
    float angle = atan(dir.y, dir.x);
    float swirl = dist * 5.0;
    angle += swirl * sin(time_f) * pingPong(dist + time_f, 1.0);
    uv = center + vec2(cos(angle), sin(angle)) * dist;
    vec4 texColor = texture(samp, uv);
    float hue = mod(angle / (2.0 * 3.14159265) + time_f * 0.1, 1.0);
    vec3 rainbow = hue2rgb(hue);
    color = vec4(texColor.rgb * rainbow, 2.0);
}

