#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    float mirrorAmount = sin(time_f * 3.14159);
    vec2 mirroredTC = vec2(abs(mirrorAmount * (tc.x - 0.5)) + 0.5 * sign(mirrorAmount), tc.y);
    color = texture(samp, mirroredTC);
}
