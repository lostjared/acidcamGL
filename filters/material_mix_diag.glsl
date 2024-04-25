#version 330 core

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform sampler2D mat_samp;
uniform float time_f;

void main() {
    
    color = texture(samp, tc);
    vec2 uv = tc;
    uv.x -= 0.05;
    uv.y -= 0.05;
    vec4 color2 = texture(mat_samp, uv);
    color = mix(color, color2, 0.5);
}

