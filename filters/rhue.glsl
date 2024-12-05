#version 330

in vec2 tc;
out vec4 color;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

vec2 rotateCoords(vec2 coords, float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return vec2(c * coords.x - s * coords.y, s * coords.x + c * coords.y);
}

vec4 adjustHue(vec4 color, float angle) {
    float U = cos(angle);
    float W = sin(angle);
    mat3 rotationMatrix = mat3(
        0.299, 0.587, 0.114,
        0.299, 0.587, 0.114,
        0.299, 0.587, 0.114
    ) + mat3(
        0.701, -0.587, -0.114,
        -0.299, 0.413, -0.114,
        -0.3, -0.588, 0.886
    ) * U + mat3(
        0.168, 0.330, -0.497,
        -0.328, 0.035, 0.292,
        1.25, -1.05, -0.203
    ) * W;
    return vec4(rotationMatrix * color.rgb, color.a);
}

void main() {
    vec2 uv = (tc - 0.5) * 2.0;
    uv = rotateCoords(uv, time_f);
    vec4 sampledColor = texture(samp, uv * 0.5 + 0.5);
    vec4 rotatedColor = adjustHue(sampledColor, length(uv) * time_f);
    color = rotatedColor;
}
