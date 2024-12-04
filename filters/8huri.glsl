#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

vec2 vortexDistortion(vec2 uv, float time) {
    vec2 center = vec2(0.5, 0.5);
    vec2 offset = uv - center;
    float distance = length(offset);
    float angle = atan(offset.y, offset.x) + sin(time + distance * 10.0) * 0.5;
    return center + vec2(cos(angle), sin(angle)) * distance;
}

void main(void) {
    vec2 uv = tc;
    uv = vortexDistortion(uv, time_f);
    uv.x = mod(uv.x + sin(time_f) * 0.2, 1.0);
    uv.y = mod(uv.y + cos(time_f) * 0.2, 1.0);
    vec4 texColor = texture(samp, uv);
    texColor.r += sin(time_f * 10.0) * 0.1;
    texColor.g += cos(time_f * 15.0) * 0.1;
    texColor.b += sin(time_f * 20.0) * 0.1;
    float[16] rVals = float[16](0.0, 0.26, 0.5, 0.74, 1.0, 0.88, 0.62, 0.38, 0.14, 0.08, 0.22, 0.54, 0.78, 0.92, 0.96, 0.1);
    float[16] gVals = float[16](0.0, 0.13, 0.25, 0.37, 0.5, 0.44, 0.31, 0.19, 0.08, 0.13, 0.37, 0.44, 0.56, 0.69, 0.82, 0.96);
    float[16] bVals = float[16](0.0, 0.06, 0.13, 0.25, 0.38, 0.44, 0.56, 0.69, 0.82, 0.96, 0.74, 0.5, 0.25, 0.13, 0.06, 0.0);
    int idxR = int(floor(texColor.r * 15.0 + 0.5));
    int idxG = int(floor(texColor.g * 15.0 + 0.5));
    int idxB = int(floor(texColor.b * 15.0 + 0.5));
    vec3 quantizedColor = vec3(rVals[idxR], gVals[idxG], bVals[idxB]);
    color = vec4(quantizedColor, texColor.a);
}

