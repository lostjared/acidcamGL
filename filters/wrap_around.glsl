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

uniform vec4 inc_valuex;
uniform vec4 inc_value;

uniform float restore_black;
in float restore_black_value;

void main(void)
{
    if(restore_black_value == 1.0 && texture(samp, tc) == vec4(0, 0, 0, 1))
        discard;
    color = texture(samp, tc);
    ivec4 source = ivec4(color * 255);
    ivec4 inc = ivec4(inc_valuex);
    ivec4 inc_v = ivec4(inc_value);
    
    source += inc + inc_v;
    
    for(int i = 0; i < 3; ++i) {
        source[i] = source[i]%255;
        color[i] = alpha * float(source[i])/255;
    }
}



