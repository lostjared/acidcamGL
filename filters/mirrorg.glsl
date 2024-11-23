#version 330 core

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

out vec4 color;

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution;
    vec2 centeredUV = (uv - 0.5) * 2.0;

    float radius = length(centeredUV);
    float mask = smoothstep(1.0, 0.9, radius);

    float angle = atan(centeredUV.y, centeredUV.x) + time_f * 0.3;
    float refractAmount = pow(1.0 - radius, 2.0) * 0.2;

    vec2 refractedUV = vec2(
        centeredUV.x + sin(angle) * refractAmount,
        centeredUV.y + cos(angle) * refractAmount
    );

    vec2 crystalUV = refractedUV * 0.5 + 0.5;
    vec4 texColorBall = texture(samp, crystalUV);
    vec4 texColorBackground = texture(samp, uv);

    color = mix(texColorBackground, texColorBall, mask);
}
