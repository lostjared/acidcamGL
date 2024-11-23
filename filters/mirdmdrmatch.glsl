#version 330 core

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

out vec4 color;

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution;
    vec2 centeredUV = uv * 2.0 - 1.0;

    float speedFactor = sin(time_f * 0.5) * 0.5 + 0.5;
    float angle1 = time_f * (0.2 + speedFactor * 0.8);
    float angle2 = time_f * (0.2 + (1.0 - speedFactor) * 0.8);

    float angle = centeredUV.x >= 0.0 ? angle1 : angle2;

    mat2 rotation = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
    centeredUV = rotation * centeredUV;

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

