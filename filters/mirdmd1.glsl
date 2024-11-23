#version 330 core

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

out vec4 color;

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution;
    vec2 centeredUV = uv * 2.0 - 1.0;

    float speedFactorX = sin(time_f * 0.5) * 0.5 + 0.5;
    float speedFactorY = cos(time_f * 0.5) * 0.5 + 0.5;

    float angleX = time_f * (0.2 + speedFactorX * 0.8);
    float angleY = time_f * (0.2 + speedFactorY * 0.8);

    centeredUV.x = abs(centeredUV.x);
    centeredUV.y = abs(centeredUV.y);

    mat2 rotationX = mat2(cos(angleX), -sin(angleX), sin(angleX), cos(angleX));
    mat2 rotationY = mat2(cos(angleY), -sin(angleY), sin(angleY), cos(angleY));

    vec2 rotatedUV = centeredUV;
    rotatedUV.x = (rotationX * vec2(rotatedUV.x, 0.0)).x;
    rotatedUV.y = (rotationY * vec2(0.0, rotatedUV.y)).y;

    vec2 mirroredUV = mod(rotatedUV, 1.0);
    vec4 texColor = texture(samp, mirroredUV);

    float pulse = 0.5 + 0.5 * sin(time_f * 3.0);
    vec3 pulsatingColor = vec3(
        0.5 + 0.5 * sin(time_f + uv.x * 10.0),
        0.5 + 0.5 * cos(time_f + uv.y * 10.0),
        0.5 + 0.5 * sin(time_f + (uv.x + uv.y) * 10.0)
    ) * pulse;

    color = vec4(texColor.rgb * 0.7 + pulsatingColor * 0.3, texColor.a);
}

