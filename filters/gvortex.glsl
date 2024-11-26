#version 330 core

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

out vec4 color;

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution;
    vec2 centeredUV = (uv - 0.5) * 2.0;

    float radius = length(centeredUV);
    float angle = atan(centeredUV.y, centeredUV.x) + time_f * 0.3;

    float mirrors = 6.0;
    angle = mod(angle, 2.0 * 3.14159 / mirrors) * mirrors;

    float refractAmount = pow(1.0 - radius, 2.0) * 0.5;
    vec2 refractedUV = vec2(
        centeredUV.x + sin(angle) * refractAmount,
        centeredUV.y + cos(angle) * refractAmount
    );

    vec2 texUV = refractedUV * 0.5 + 0.5;

    vec4 texColor = texture(samp, mod(texUV, 1.0));

    float pulse = 0.5 + 0.5 * sin(time_f * 3.0);
    vec3 dynamicColor = vec3(
        0.5 + 0.5 * sin(time_f + uv.x * 10.0),
        0.5 + 0.5 * cos(time_f + uv.y * 10.0),
        0.5 + 0.5 * sin(time_f + (uv.x + uv.y) * 10.0)
    );

    color = vec4(texColor.rgb * 0.6 + dynamicColor * 0.4 * pulse, texColor.a);
}

