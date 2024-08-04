#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec4 texColor = texture(samp, tc);
    float r = texColor.r;
    float g = texColor.g;
    float b = texColor.b;
    color = vec4(
        dot(vec3(r, g, b), vec3(0.393, 0.769, 0.189)),
        dot(vec3(r, g, b), vec3(0.349, 0.686, 0.168)),
        dot(vec3(r, g, b), vec3(0.272, 0.534, 0.131)),
        texColor.a
    );
}
