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

vec4 clipColor(vec4 colorx) {
    ivec4 rgb;
    for(int i = 0; i < 3; ++i) {
        rgb[i] = int(colorx[i] * 255);
        if(rgb[i] > 255)
            rgb[i] = rgb[i]%255;
    }
    vec4 col;
    for(int i = 0; i < 3; ++i)
        col[i] = float(rgb[i])/255;
    return col;
}

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
    ivec4 source = ivec4(color * 255);
    vec4 color2;
    vec2 pos = gl_FragCoord.xy;
    color2 = texture(mat_samp, tc);
    color = (0.5 * color) + (0.5 * (color2 * random(pos*timeval)));
    color = xor_RGB(color, source);
}

