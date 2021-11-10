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
in vec2 iResolution_;
uniform vec2 iResolution;
uniform float restore_black;
in float restore_black_value;

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

void main(void)
{
    if(restore_black_value == 1.0 && texture(samp, tc) == vec4(0, 0, 0, 1))
        discard;
    color = texture(samp, tc);
    vec4 color2;
    color2 = texture(mat_samp, tc);

    vec2 st = (gl_FragCoord.xy / iResolution_.xy);
    
    color2[0] = color2[2] + st.x;
    color2[1] = color2[1] + st.y;
    color2[2] = color2[0] + st.x+st.y;
    
    ivec4 color_1, color_2;
    color_1 = ivec4(color * 255);
    color_2 = ivec4(color2 * 255);
    for(int i = 0; i < 3; ++i) {
        color_1[i] = color_1[i]+color_2[i];
        color_1[i] = color_1[i]^color_2[i];
        color[i] = (0.5 * color[i]) + (0.5 * float(color_1[i])/255);
        color[i] = color[i] * st[0];
    }
}




