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
uniform vec4 inc_value;

uniform float restore_black;
in float restore_black_value;

void main(void)
{
    if(restore_black_value == 1.0 && texture(samp, tc) == vec4(0, 0, 0, 1))
        discard;
    
    color = texture(samp, tc);
    ivec4 source = ivec4(255 * color);
    vec2 vpos;
    vpos[0] = 1.0-tc[0];
    vpos[1] = tc[1];
    vec4 color2 = texture(samp, vpos);
    color = (0.5 * color) + (0.5 * color2);
    vec4 color2x = texture(samp, tc / 2);
    color2x[0] = 0.1;
    vec4 color3 = texture(samp, tc/ 4);
    color3[1] = 0.1;
    vec4 color4 = texture(samp, tc/ 8);
    color4[2] = 0.1;
    color = (color * 0.3) + (color2x * 0.3) + (color3 * 0.3) + (color4 * 0.3) ;
}




