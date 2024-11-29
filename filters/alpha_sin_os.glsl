#version 330 core

out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float pingPong(float value, float range) {
    float modValue = mod(value, 2.0 * range);
    return range - abs(modValue - range);
}


void main() {
    vec2 uv = tc;
    vec4 pixelColor = texture(samp, uv);
    float scale = 0.5 + 0.5 * pingPong(time_f, 8.0) + 1.5;
    vec4 scaledColor = mod(pixelColor * scale, 1.0);
    color = scaledColor;
}
