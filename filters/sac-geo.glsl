#version 330 core

in vec2 tc;
out vec4 color;
uniform float time_f;
uniform vec2 iResolution;
uniform sampler2D samp;

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 sacredGeometry(vec2 uv, float time) {
    float angle = atan(uv.y, uv.x);
    float radius = length(uv);
    float frequency = 5.0;
    float amplitude = 0.5;

    float sacredPattern = sin(radius * frequency + time) * amplitude + 0.5;
    float hue = fract(sacredPattern + time * 0.1);
    float saturation = 1.0;
    float value = 1.0;

    return hsv2rgb(vec3(hue, saturation, value));
}

void main() {
    vec2 uv = tc * 2.0 - 1.0;
    uv *= vec2(iResolution.x / iResolution.y, 1.0);
    float time = time_f * 0.1;

    vec3 sacredColor = sacredGeometry(uv, time);

    vec2 complexCoord = uv * vec2(cos(time), sin(time));
    vec3 complexColor = sacredGeometry(complexCoord, time);

    vec4 textureColor = texture(samp, tc);
    vec3 combinedColor = mix(sacredColor, complexColor, 0.5);

    color = vec4(mix(textureColor.rgb, combinedColor, 0.5), 1.0);
}
