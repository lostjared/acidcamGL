#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec2 rotate(vec2 p, float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return vec2(c * p.x - s * p.y, s * p.x + c * p.y);
}

void main() {
    vec2 uv = tc;
    float squareSize = sin(0.2 * time_f);
    vec2 gridCoord = floor(uv / squareSize);
    vec2 localCoord = fract(uv / squareSize);

    float diag = mod(gridCoord.x + gridCoord.y, 2.0);
    

    if (diag == 0.0) {
        float angle = time_f + gridCoord.x * 0.5 + gridCoord.y * 0.5;
        localCoord = rotate(localCoord - 0.5, angle) + 0.5;
        uv = fract(gridCoord * squareSize + localCoord);
    }

    vec4 texColor = texture(samp, uv);
    color = texColor;
}

