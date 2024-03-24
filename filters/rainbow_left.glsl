#version 330 core

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

vec3 hsv2rgb(vec3 c) {
    c.x = fract(c.x);     vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    vec2 uv = tc;
    float timeScale = 0.2;
    float spatialScale = 3.0;
    float hue = uv.x * spatialScale + time_f * timeScale;
    vec3 colorHue = hsv2rgb(vec3(hue, 1.0, 1.0));
    vec4 texColor = texture(samp, uv);
    vec4 finalColor = vec4(texColor.rgb * colorHue, texColor.a);
    color = finalColor;
}
