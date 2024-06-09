#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform sampler2D mat_samp;
uniform float time_f;

void main(void) {
    vec2 wave_tc = tc + vec2(sin(tc.y * 10.0 + time_f), cos(tc.x * 10.0 + time_f)) * 0.05;
    vec4 color_samp = texture(samp, wave_tc);
    vec4 color_mat_samp = texture(mat_samp, wave_tc);
    
    vec4 texture_color = mix(color_samp, color_mat_samp, 0.5);
    
    color = texture_color;
}
