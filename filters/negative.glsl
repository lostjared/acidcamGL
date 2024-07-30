#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec4 texColor = texture(samp, tc);
    color = vec4(1.0 - texColor.rgb, texColor.a);
}
