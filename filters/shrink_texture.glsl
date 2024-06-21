#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec2 uv = tc;
    vec2 pos = uv - 0.5;
    float len = length(pos);
    
    vec2 ripple = pos * (0.6 / len) * sin(len * 2.0 - time_f * 1.0);
    uv += ripple * sin(0.3 * time_f);
    
    color = texture(samp, uv);
}
