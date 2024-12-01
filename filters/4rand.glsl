
#version 330 core

in vec2 tc;
out vec4 color;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

// Function to quantize colors to an 8-bit palette
vec3 NESPalette(vec3 col) {
    float[16] rVals = float[16](0.0, 0.26, 0.5, 0.74, 1.0, 0.88, 0.62, 0.38, 0.14, 0.08, 0.22, 0.54, 0.78, 0.92, 0.96, 0.1);
    float[16] gVals = float[16](0.0, 0.13, 0.25, 0.37, 0.5, 0.44, 0.31, 0.19, 0.08, 0.13, 0.37, 0.44, 0.56, 0.69, 0.82, 0.96);
    float[16] bVals = float[16](0.0, 0.06, 0.13, 0.25, 0.38, 0.44, 0.56, 0.69, 0.82, 0.96, 0.74, 0.5, 0.25, 0.13, 0.06, 0.0);
    int idxR = int(floor(col.r * 15.0 + 0.5));
    int idxG = int(floor(col.g * 15.0 + 0.5));
    int idxB = int(floor(col.b * 15.0 + 0.5));
    return vec3(rVals[idxR], gVals[idxG], bVals[idxB]);
}

// Function to apply a feedback loop
vec4 echo_loop(vec4 baseColor, int count) {
    vec4 col = baseColor;
    for (int i = 2; i < count; i += 2) {
        vec4 pos = texture(samp, tc / (i * fract(time_f)));
        col = (0.6 * col) + (0.3 * pos);
    }
    return col;
}

// Main function
void main(void) {
    vec2 normCoord = tc;

    // Apply deformation effect
    float warpAmount = tan(time_f);
    vec2 warp = vec2(
        sin(normCoord.y * 10.0 + time_f) * warpAmount,
        cos(normCoord.x * 10.0 + time_f) * warpAmount
    );
    vec2 warpedCoord = normCoord + warp;

    // Sample texture with deformation
    vec4 texColor = texture(samp, warpedCoord);

    // Apply 8-bit palette quantization
    vec3 quantizedColor = NESPalette(texColor.rgb);
    vec4 quantizedTexColor = vec4(quantizedColor, texColor.a);

    // Apply echo loop effect
    vec4 echoedColor = echo_loop(quantizedTexColor, 14);

    // Apply negative effect
    color = vec4(1.0 - echoedColor.rgb, echoedColor.a);
}

