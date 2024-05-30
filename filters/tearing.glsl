#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;

void main(void) {
    float tear1 = sin(tc.x * 10.0 + time_f * 5.0) * 0.05;
    float tear2 = sin(tc.x * 20.0 + time_f * 7.0) * 0.03;
    float tear3 = sin(tc.x * 30.0 + time_f * 9.0) * 0.02;
    
    float combined_tear = tear1 + tear2 + tear3;
    vec2 new_tc = vec2(tc.x + combined_tear, tc.y + combined_tear);
    color = texture(samp, new_tc);
}
