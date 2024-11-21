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

    float pulse = sin(time_f * 2.0 + dist * 10.0) * 0.15;
    float expansion = cos(time_f * 3.0 - dist * 15.0) * 0.2;
    float spiral = sin(atan(offset.y, offset.x) * 3.0 + time_f * 2.0) * 0.1;

    vec2 morphUV = uv + normalize(offset) * (pulse + expansion);
    morphUV += vec2(cos(atan(offset.y, offset.x)), sin(atan(offset.y, offset.x))) * spiral;

    vec4 texColor = texture(samp, morphUV * 0.5 + 0.5);
    texColor.rgb *= 1.0 + 0.3 * sin(dist * 20.0 - time_f * 5.0);

    color = vec4(texColor.rgb, texColor.a);
}

