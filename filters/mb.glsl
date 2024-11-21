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

    float pulse = sin(time_f * 2.0 + dist * 10.0) * 0.15;
    float spiral = sin(angle * 5.0 + time_f * 2.0) * 0.15;
    float stretch = cos(time_f * 3.0 - dist * 15.0) * 0.2;

    vec2 spiralOffset = vec2(cos(angle), sin(angle)) * spiral;
    vec2 stretchOffset = normalize(offset) * (pulse + stretch);
    vec2 morphUV = uv + spiralOffset + stretchOffset;

    vec4 texColor = texture(samp, morphUV * 0.5 + 0.5);

    vec3 blended = vec3(0.0);
    float weight = 0.0;

    for (float x = -2.0; x <= 2.0; x++) {
        for (float y = -2.0; y <= 2.0; y++) {
            vec2 sampleUV = morphUV + vec2(x, y) * 0.005;
            vec4 sampleColor = texture(samp, sampleUV * 0.5 + 0.5);
            blended += sampleColor.rgb;
            weight += 1.0;
        }
    }

    blended /= weight;
    blended = mix(texColor.rgb, blended, 0.5);

    float shimmer = sin(dist * 20.0 - time_f * 5.0) * 0.1 + 1.0;
    color = vec4(blended * shimmer, texColor.a);
}

