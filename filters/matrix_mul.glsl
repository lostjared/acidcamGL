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


vec4 xor_RGB(vec4 icolor, ivec4 isource) {
    ivec3 int_color;
    for(int i = 0; i < 3; ++i) {
        int_color[i] = int(255 * icolor[i]);
        int_color[i] = int_color[i]^isource[i];
        if(int_color[i] > 255)
            int_color[i] = int_color[i]%255;
        icolor[i] = float(int_color[i])/255;
    }
    return icolor;
}

void main(void)
{
    if(restore_black_value == 1.0 && texture(samp, tc) == vec4(0, 0, 0, 1))
        discard;
    color = texture(samp, tc);
    ivec4 source = ivec4(255 * color);
    mat4 matrix = mat4(alpha, 0.1, 0.2, 0.3,
                       0.4, -alpha, 0.5, 0.6,
                       0.7, 0.8, alpha, 0.9,
                       0, 0, 0, -alpha);
    
    color = color * (0.5 * matrix);
    color = xor_RGB(color, source);
    vec4 color2 = texture(samp, tc+0.01);
    vec4 color3 = texture(samp, tc-0.01);
    color = (0.5 * color) + (0.3 * color2) + (0.3 * color3);
}




