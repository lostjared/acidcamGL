#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform sampler2D mat_samp;
uniform float time_f;
uniform vec2 iResolution;

void main() {
    vec2 uv = tc;
    float distortA = sin(time_f * 0.5) * 0.1;
    float distortB = cos(time_f * 0.5) * 0.1;

    vec2 uv1 = vec2(uv.x + distortA, uv.y + distortB);

    vec2 uv2 = vec2(uv.x - distortA, uv.y - distortB);

    vec4 texColor1 = texture(samp, tan(uv1));
    vec4 texColor2 = texture(mat_samp, sin(uv2));
    float mixFactor = 0.5 + 0.5 * sin(time_f * 0.7);
    vec4 mixedTexture = mix(texColor1, texColor2, mixFactor);
    color = mixedTexture;
}
