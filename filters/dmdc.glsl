#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

float pingPong(float t, float l) {
    return abs(mod(t, 2.0 * l) - l);
}

mat2 rotate(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat2(c, -s, s, c);
}

void main() {
    vec2 uv = tc * iResolution.xy;
    vec2 centerUV = (floor(uv / 50.0) + 0.5) * 50.0;
    vec2 localUV = uv - centerUV;
    float t = pingPong(time_f, 4.0) * 2.0;
    float rotation = atan(localUV.y, localUV.x) + length(localUV) * 0.1 + t;
    localUV = rotate(rotation) * localUV;
    vec2 diamondMask = abs(mod(localUV, 100.0) - 50.0);
    float mask = smoothstep(25.0, 30.0, length(diamondMask - vec2(25.0)));
    vec2 texCoord = (centerUV + localUV) / iResolution.xy;
    vec4 sampledColor = texture(samp, texCoord);
    color = mix(sampledColor, vec4(0.0), mask);
}
