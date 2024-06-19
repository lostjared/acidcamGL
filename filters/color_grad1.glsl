#version 330 core
in vec2 tc;
out vec4 color;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;
uniform float restore_black;
in float restore_black_value;
uniform vec4 inc_valuex;

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

void main(void)
{
    if(restore_black_value == 1.0 && texture(samp, tc) == vec4(0, 0, 0, 1))
        discard;
    color = texture(samp, tc);
    vec2 pos = gl_FragCoord.xy / iResolution;
    vec4 s = color * sin(inc_valuex / 255.0 * time_f);
    color[0] += s[0] * pos[0];
    color[1] += s[1] * pos[1];
 
    float time_t = pingPong(time_f, 20) + 2.0;
    
    color = sin(color * time_t);
}
