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
    color = texture(samp, tc) * texture(mat_samp, tc);
    float x = gl_FragCoord.x;
    float y = gl_FragCoord.y;
    vec2 cord1 = vec2(tc[0]/x, tc[1]/y);
    vec2 cord2 = vec2(tc[0]/x+32, tc[1]/y+16);
    vec2 cord3 = vec2(tc[0]/x+64, tc[1]/y+32);
    vec4 col1 = texture(samp, cord1) * texture(mat_samp, cord1);
    vec4 col2 = texture(samp, cord2) * texture(mat_samp, cord2);
    vec4 col3 = texture(samp, cord3) * texture(mat_samp, cord3);
    color[0] = color[0] + col1[0];
    color[1] = color[1] + col2[1];
    color[2] = color[2] + col3[2];
    color = color * alpha;
}
