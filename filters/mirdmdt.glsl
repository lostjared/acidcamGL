#version 330 core

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

out vec4 color;

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution;
    vec2 centeredUV = uv * 2.0 - 1.0;

    float angle = time_f * 0.5;
    mat2 rotation = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));

    if (centeredUV.x < 0.0) {
        centeredUV.x = -centeredUV.x;
        centeredUV = rotation * centeredUV;
    } else {
        centeredUV = rotation * centeredUV;
    }

    centeredUV = mod(centeredUV, 1.0);
    vec2 mirroredUV = centeredUV * 0.5 + 0.5;
    vec4 texColor = texture(samp, mirroredUV);

    color = texColor;
}
