#version 330
in vec2 tc;
out vec4 color;
in float alpha_r;
in float alpha_g;
in float alpha_b;
in float current_index;
in float timeval;
in float alpha;
in vec3 vpos;
in vec4 optx_val;
uniform vec4 optx;
in vec4 random_value;
uniform vec4 random_var;
uniform float alpha_value;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform sampler2D samp;
uniform float value_alpha_r, value_alpha_g, value_alpha_b;
uniform float index_value;
uniform float time_f;
uniform float restore_black;
in float restore_black_value;

void main(void)
{
    if(restore_black_value == 1.0 && texture(samp, tc) == vec4(0, 0, 0, 1))
        discard;
    color = texture(samp, tc);
    vec4 color2 = texture(samp, tc+0.01);
    vec4 color3 = texture(samp, tc-0.01);
    vec2 pos = tc;
    pos[0] -= 0.01;
    vec2 pos2 = tc;
    pos2[1] += 0.01;
    vec4 color4 = texture(samp, pos);
    vec4 color5 = texture(samp, pos2);
    
    color = (color + color2 + color3 + color4 + color5) / 5;
}





