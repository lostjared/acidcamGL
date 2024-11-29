#version 330 core

out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main() {
    vec2 uv = tc;
    float randVal = fract(sin(dot(uv * time_f, vec2(12.9898, 78.233))) * 43758.5453);
    vec4 ghostTrail = texture(samp, uv + vec2(0.01 * randVal, 0.01 * randVal));
    vec4 inverted = vec4(1.0) - ghostTrail;
    color = mix(ghostTrail, inverted, 0.5);
}

