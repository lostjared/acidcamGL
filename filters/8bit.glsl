#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

vec3 NESPalette(vec3 col) {
    float[16] rVals = float[16](0.0, 0.26, 0.5, 0.74, 1.0, 0.88, 0.62, 0.38, 0.14, 0.08, 0.22, 0.54, 0.78, 0.92, 0.96, 0.1);
    float[16] gVals = float[16](0.0, 0.13, 0.25, 0.37, 0.5, 0.44, 0.31, 0.19, 0.08, 0.13, 0.37, 0.44, 0.56, 0.69, 0.82, 0.96);
    float[16] bVals = float[16](0.0, 0.06, 0.13, 0.25, 0.38, 0.44, 0.56, 0.69, 0.82, 0.96, 0.74, 0.5, 0.25, 0.13, 0.06, 0.0);

    int idxR = int(floor(col.r * 15.0 + 0.5));
    int idxG = int(floor(col.g * 15.0 + 0.5));
    int idxB = int(floor(col.b * 15.0 + 0.5));

    return vec3(rVals[idxR], gVals[idxG], bVals[idxB]);
}

void main(void) {
    vec2 pixelSize = vec2(256.0, 240.0);
    vec2 coord = floor(tc * pixelSize) / pixelSize;
    vec4 texColor = texture(samp, coord);
    vec3 quantizedColor = NESPalette(texColor.rgb);
    color = vec4(quantizedColor, texColor.a);
}
