#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform sampler2D mat_samp;
uniform float time_f;
uniform vec2 iResolution;
void main(void) {
    vec2 uv = tc;
   float blockSize = 4.0 + 60.0 * abs(sin(time_f));

    vec2 blockUV = floor(uv * iResolution / blockSize) * blockSize / iResolution;

    vec4 color1 = texture(samp, blockUV);
    vec4 color2 = texture(mat_samp, blockUV);

    float mixFactor = 0.5 + 0.5 * sin(time_f * 3.14159);
    vec4 mixedColor = mix(color1, color2, mixFactor);

    color = mixedColor;
}
