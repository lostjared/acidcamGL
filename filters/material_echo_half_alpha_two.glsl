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
    color = texture(samp, tc);
    vec4 color2 = texture(samp, tc /(0.9 * alpha));
    vec4 color3 = texture(samp, tc/ (1.5 * alpha));
    vec4 color4 = texture(samp, tc/ (2.0 * alpha));
    
    vec4 color2a = texture(mat_samp, tc /(0.9 * alpha));
    vec4 color3a = texture(mat_samp, tc/ (1.5 * alpha));
    vec4 color4a = texture(mat_samp, tc/ (2.0 * alpha));

    color2 = (0.5 * color2) + (0.5 * color2a);
    color3 = (0.5 * color3) + (0.5 * color3a);
    color4 = (0.5 * color4) + (0.5 * color4a);
    
    color = (color * 0.4) + (color2 * 0.4) + (color3 * 0.4) + (color4 * 0.4) ;
}








