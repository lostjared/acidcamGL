#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

float radialPattern(vec2 uv, float rings, float spokes) {
    vec2 centered_uv = uv - 0.5;
    float r = length(centered_uv);
    float circle = smoothstep(0.02, 0.03, sin(rings * r * 6.28 + time_f * 0.5));
    float angle = atan(centered_uv.y, centered_uv.x);
    float spoke = smoothstep(0.02, 0.03, sin(spokes * angle * 0.5 + time_f * 0.2));
    return circle + spoke;
}

vec2 warpTextureCoords(vec2 uv, float webPattern) {
    float pingPongOffset = pingPong(time_f * 0.2, 0.5);
    vec2 warpFactor = vec2(1.0 + webPattern * 0.1, 1.0 - webPattern * 0.1);
    vec2 warpedUV = uv * warpFactor;
    warpedUV += 0.02 * vec2(sin(uv.y * 10.0 + pingPongOffset), cos(uv.x * 10.0 + pingPongOffset));
    return warpedUV;
}

void main(void) {
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    float web = radialPattern(uv, 10.0, 12.0);
    vec2 warpedTC = warpTextureCoords(tc, web);
    vec4 textureColor = texture(samp, warpedTC);
    color = textureColor * vec4(1.0, 1.0, 1.0, 1.0) * (0.5 + web * 0.5);
}
