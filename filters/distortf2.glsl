#version 330 core

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;
void main() {
    vec2 uv = tc;
    vec2 center = vec2(0.5, 0.5);
    vec2 offset = uv - center;
    float dist = length(offset);

    float pulse = sin(dist * 30.0 - time_f * 10.0) * 0.1;
    vec2 pulseUV = uv + pulse * normalize(offset);
    float fractalFactor = cos(length(offset) * 15.0 + time_f * 5.0) * 0.2;
    vec2 fractalUV = pulseUV + fractalFactor * normalize(offset);
    vec2 explosionUV = fractalUV + dist * vec2(cos(time_f * 4.0), sin(time_f * 4.0)) * 0.2;
    vec3 rainbow = vec3(0.5 + 0.5 * sin(time_f + explosionUV.x * 10.0),
                        0.5 + 0.5 * sin(time_f + explosionUV.y * 10.0),
                        0.5 + 0.5 * sin(time_f * 2.0));
    float angle = atan(offset.y, offset.x) + sin(time_f * 3.0) * 2.0;
    vec2 swirlUV = center + dist * vec2(cos(angle), sin(angle)) * 1.5;

    vec2 combinedUV = mix(swirlUV, explosionUV, 0.7);
    vec4 texColor = texture(samp, combinedUV);
    vec3 intenseColor = texColor.rgb * rainbow * 2.0;
    float glow = exp(-10.0 * dist) * 2.0;
    vec3 finalColor = mix(intenseColor, vec3(glow), 0.4);

    color = vec4(finalColor, texColor.a);
}

