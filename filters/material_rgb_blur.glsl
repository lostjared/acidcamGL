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
    vec2 pos1 = tc;
    vec2 pos2 = tc;
    pos1 += 0.01;
    pos2 += 0.01;
    vec4 color1,color1m,color2,color2m;
    color1 = texture(samp, pos1);
    color1m = texture(mat_samp, pos1);
    color2 = texture(samp, pos2);
    color2m = texture(mat_samp, pos2);
    color[1] = (0.5 * color1[1]) + (0.5 * color1m[1]);
    color[2] = (0.5 * color2[2]) + (0.5 * color2m[2]);
}




