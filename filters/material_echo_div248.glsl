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
uniform sampler2D mat_samp;
uniform float value_alpha_r, value_alpha_g, value_alpha_b;
uniform float index_value;
uniform float time_f;


uniform float restore_black;
in float restore_black_value;

void main(void)
{
    if(restore_black_value == 1.0 && texture(samp, tc) == vec4(0, 0, 0, 1))
        discard;
    color = (0.5 * texture(samp, tc)) + (0.5 * texture(mat_samp, tc));
    
    vec2 tc1 = tc;
    vec2 tc2 = tc;
    
    tc1[0] = 1.0-tc1[0];
    tc2[1] = 1.0-tc2[1];
    
    vec4 color2 = texture(samp, tc1/2);
    vec4 color3 = texture(mat_samp,tc/4);
    vec4 color4 = texture(samp, tc2/8);
    color = (color * 0.4) + (color2 * 0.4) + (color3 * 0.4) + (color4 * 0.4) ;
}




