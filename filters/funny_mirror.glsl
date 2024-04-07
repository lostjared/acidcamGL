#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    
    float loop_dur = 15.0;
    float current_t = mod(time_f, loop_dur);
 
    vec2 uv = (tc - 0.5) * (iResolution.x / iResolution.y, 1.0);
    float radius = length(uv) * current_t;
    vec2 coord = sin(tc * radius/2);
       
    
    color = texture(samp, coord);
}
