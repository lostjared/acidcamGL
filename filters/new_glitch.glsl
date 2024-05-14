#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;
uniform float value_alpha_r, value_alpha_g, value_alpha_b;

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(void) {
    vec2 pos = tc;
    
    vec2 randomOffset = vec2(rand(pos + time_f), rand(pos - time_f));
    
    pos.x += (value_alpha_r * randomOffset.x - 0.5) * 0.05;
    pos.y += (value_alpha_g * randomOffset.y - 0.5) * 0.05;
    
    vec4 texColor = texture(samp, pos);
    vec4 shiftedColor1 = texture(samp, pos + vec2(value_alpha_b * 0.01, 0.0));
    vec4 shiftedColor2 = texture(samp, pos + vec2(0.0, value_alpha_b * 0.01));
    
    color = mix(texColor, (shiftedColor1 + shiftedColor2) * 0.5, 0.5);
}