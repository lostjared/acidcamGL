#version 330 core

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main() {
    vec2 uv = tc * 2.0 - 1.0;
    uv *= iResolution.y / iResolution.x;
    vec2 center = vec2(0.0, 0.0);
    vec2 offset = uv - center;
    float dist = length(offset);
    float angle = atan(offset.y, offset.x);

    float ripple = sin(dist * 30.0 - time_f * 10.0) * 0.05;
    float spiral = sin(angle * 5.0 + time_f) * 0.05;
    float wave = sin(uv.x * 10.0 + time_f) * 0.03;

    vec2 warpedUV = uv + normalize(offset) * ripple;
    warpedUV += vec2(cos(angle), sin(angle)) * spiral;
    warpedUV.y += wave;

    vec4 texColor = texture(samp, warpedUV * 0.5 + 0.5);
    float shimmer = sin(dist * 10.0 + time_f * 5.0) * 0.1 + 0.9;
    vec3 finalColor = texColor.rgb * shimmer;

    color = vec4(finalColor, texColor.a);
}

