#version 330 core

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main() {
    vec2 uv = tc;

    float trailStrength = 0.05;
    float strobeSpeed = sin(time_f * 10.0);

    vec2 trailOffsetR = vec2(trailStrength * strobeSpeed, 0.0);
    vec2 trailOffsetG = vec2(-trailStrength * strobeSpeed, trailStrength * strobeSpeed);
    vec2 trailOffsetB = vec2(0.0, -trailStrength * strobeSpeed);

    vec3 rgbTrail = vec3(
        texture(samp, uv + trailOffsetR).r,
        texture(samp, uv + trailOffsetG).g,
        texture(samp, uv + trailOffsetB).b
    );

    float strobe = 0.5 + 0.5 * sin(time_f * 20.0);
    vec3 baseColor = texture(samp, uv).rgb;

    color = vec4(mix(baseColor, rgbTrail, strobe), 1.0);
}

