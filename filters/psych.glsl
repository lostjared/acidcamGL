#version 330 core

in vec2 tc;
out vec4 color;
uniform float time_f;
uniform vec2 iResolution;
uniform sampler2D samp;

void main() {
    vec2 uv = (tc - 0.5) * 2.0;
    uv.x *= iResolution.x / iResolution.y;
    float time = time_f * 0.5;
    float sine1 = sin(uv.x * 10.0 + time) * cos(uv.y * 10.0 + time);
    float sine2 = sin(uv.y * 15.0 - time) * cos(uv.x * 15.0 - time);
    float sine3 = sin(sqrt(uv.x * uv.x + uv.y * uv.y) * 20.0 + time);
    float pattern = sine1 + sine2 + sine3;
    float colorIntensity = pattern * 0.5 + 0.5;
    vec3 psychedelicColor = vec3(
        sin(colorIntensity * 6.28318 + 0.0) * 0.5 + 0.5,
        sin(colorIntensity * 6.28318 + 2.09439) * 0.5 + 0.5,
        sin(colorIntensity * 6.28318 + 4.18879) * 0.5 + 0.5
    );
    color = vec4(psychedelicColor, 1.0);
    color = mix(color, texture(samp, tc), 0.5);
}
