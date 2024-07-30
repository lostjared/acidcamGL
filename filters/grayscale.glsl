#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec4 texColor = texture(samp, tc);
    float gray = dot(texColor.rgb, vec3(0.299, 0.587, 0.114));
    color = vec4(vec3(gray), texColor.a);
}
