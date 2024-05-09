#version 330 core

out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main() {
    
    vec2 pos = tc;
    
    float wave_x = sin((tc.x + time_f) * 5.0);
    float wave_y = cos((tc.y + time_f) * 8.0);

    pos.x += wave_x;
    pos.y += wave_y;
    color = texture(samp, sin(pos * time_f));
}
