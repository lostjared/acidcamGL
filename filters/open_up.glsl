#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec2 uv = tc * 2.0 - 1.0;
    float dist = length(uv);
    float angle = atan(uv.y, uv.x);
    float centerSize = 0.5 + 0.4 * sin(time_f * 3.0);
    float bloomEffect = sin(time_f + dist * 6.0) * 0.5 + 0.5;

    vec2 centerUV = uv * mix(1.0, bloomEffect, 0.3);
    vec2 outerUV = uv * (1.0 + 0.5 * (1.0 - dist));

    centerUV = centerUV * 0.5 + 0.5;
    outerUV = outerUV * 0.5 + 0.5;

    vec4 centerColor = texture(samp, centerUV);
    vec4 outerColor = texture(samp, outerUV);

    float mixFactor = smoothstep(centerSize - 0.1, centerSize, dist);
    color = mix(centerColor, outerColor, mixFactor);
    color = mix(color, texture(samp, tc), 0.5);
}
