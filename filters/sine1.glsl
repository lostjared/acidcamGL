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
vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main(void) {
    vec2 uv = tc * iResolution.xy / iResolution.y;
    float pattern = sin(uv.x * 10.0) * cos(uv.y * 10.0);

    pattern = (pattern + 1.0) * 0.5;

    float reveal = pingPong(time_f * 0.5, 1.0);

    float cracks = step(pattern, reveal);

    float hue = pattern * 6.0;

    vec3 colorRGB = hsv2rgb(vec3(hue, 1.0, 1.0));

    color = vec4(colorRGB * cracks, 1.0);
    color = mix(color,texture(samp,tc), 0.5);
    
}

