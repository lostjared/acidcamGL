#version 330 core

in vec2 tc;
out vec4 color;
uniform float time_f;
uniform vec2 iResolution;
uniform sampler2D samp;

float PI = 3.1415926535897932384626433832795;

void main() {
    vec2 uv = (tc - 0.5) * 2.0;
    uv.x *= iResolution.x / iResolution.y;
    
    float plasma = 0.0;
    plasma += sin((uv.x + time_f) * 5.0);
    plasma += sin((uv.y + time_f) * 5.0);
    plasma += sin((uv.x + uv.y + time_f) * 5.0);
    plasma += cos(length(uv + time_f) * 10.0);
    plasma *= 0.25;

    vec3 baseColor;
    baseColor.r = cos(plasma * PI + time_f * 0.2) * 0.5 + 0.5;
    baseColor.g = sin(plasma * PI + time_f * 0.2) * 0.5 + 0.5;
    baseColor.b = sin(plasma * PI + time_f * 0.4) * 0.5 + 0.5;

    float dispersion = 0.02;
    vec3 prismColor;
    prismColor.r = texture(samp, tc + vec2(dispersion, 0.0)).r;
    prismColor.g = texture(samp, tc).g;
    prismColor.b = texture(samp, tc - vec2(dispersion, 0.0)).b;

    color = vec4(mix(baseColor, prismColor, 0.6), 1.0);
}

