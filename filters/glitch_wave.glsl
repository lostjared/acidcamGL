#version 330 core

in vec2 tc; // Texture coordinates
out vec4 color; // Output color
uniform sampler2D samp; // Scene texture
uniform float time_f; // Time variable for animation
uniform vec2 iResolution; // Screen resolution

void main() {
    vec2 uv = tc;
    vec2 reflectedUV = vec2(uv.x, uv.y);
    float waveAmplitude = 0.5;
    float waveFrequency = 3.0;
    float waveSpeed = 2.0;
    reflectedUV.y += sin(reflectedUV.x * waveFrequency + time_f * waveSpeed) * waveAmplitude;

    reflectedUV = clamp(reflectedUV, 0.0, 1.0);
    vec4 reflectedColor = texture(samp, reflectedUV);
    color = reflectedColor;
}
