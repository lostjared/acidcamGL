#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;

void main(void) {
     float tear = sin(tc.x * 10.0 + time_f * 5.0) * 0.05;
    vec2 new_tc = vec2(tc.x + tear, tc.y + tear);
    color = texture(samp, new_tc);
}
