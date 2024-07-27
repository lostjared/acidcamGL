#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;

void main(void) {
    vec4 texColor = texture(samp, tc);

    vec4 vhsColor = texColor;
    vhsColor.rgb = vhsColor.rgb * vec3(0.9, 0.85, 0.8) + vec3(0.1, 0.1, 0.15);
    vhsColor.rgb = clamp(vhsColor.rgb, 0.0, 1.0);

    color = vhsColor;
}
