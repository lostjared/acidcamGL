#version 330 core

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

out vec4 color;

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution;
    vec2 centeredUV = uv * 2.0 - 1.0;

    float angle1 = time_f * 0.5;
    float angle2 = -time_f * 0.5;

    if (centeredUV.x >= 0.0) {
        mat2 rotation1 = mat2(cos(angle1), -sin(angle1), sin(angle1), cos(angle1));
        centeredUV = rotation1 * centeredUV;
    } else {
        mat2 rotation2 = mat2(cos(angle2), -sin(angle2), sin(angle2), cos(angle2));
        centeredUV = rotation2 * centeredUV;
    }

    centeredUV = abs(centeredUV);
    vec2 mirroredUV = mod(centeredUV, 1.0);
    vec4 texColor = texture(samp, mirroredUV);

    float pulse = 0.5 + 0.5 * sin(time_f * 3.0);
    vec3 pulsatingColor = vec3(
        0.5 + 0.5 * sin(time_f + uv.x * 10.0),
        0.5 + 0.5 * cos(time_f + uv.y * 10.0),
        0.5 + 0.5 * sin(time_f + (uv.x + uv.y) * 10.0)
    ) * pulse;

    color = vec4(texColor.rgb * 0.7 + pulsatingColor * 0.3, texColor.a);
}

