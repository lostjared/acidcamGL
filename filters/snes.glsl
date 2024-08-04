#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec2 uv = tc;
    float pixelSize = 0.01;
    uv = floor(uv / pixelSize) * pixelSize;
    vec4 texColor = texture(samp, uv);
    vec3 paletteColor = floor(texColor.rgb * 31.0) / 31.0;
    color = vec4(paletteColor, texColor.a);
}
