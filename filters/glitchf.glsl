#version 330 core

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float random(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    vec2 uv = tc;

    float glitchStrength = 0.05 + 0.03 * sin(time_f * 3.0);
    float glitchLine = step(0.5 + 0.1 * sin(time_f * 10.0), fract(uv.y * 10.0 + time_f * 5.0));
    float displacement = glitchStrength * (random(uv + time_f) - 0.5);

    vec2 distortedUV = uv;
    distortedUV.x += displacement * glitchLine;

    vec4 texColor = texture(samp, distortedUV);

    vec2 offset = vec2(0.01 * sin(time_f * 15.0), 0.01 * cos(time_f * 10.0));
    vec3 glitchRGB = vec3(
        texture(samp, distortedUV + vec2(offset.x, 0.0)).r,
        texture(samp, distortedUV + vec2(-offset.x, offset.y)).g,
        texture(samp, distortedUV + vec2(0.0, -offset.y)).b
    );

    float strobe = step(0.5, fract(time_f * 10.0)) * 0.2;

    color = vec4(mix(texColor.rgb, glitchRGB, glitchLine + strobe), texColor.a);
}

