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

vec4 shift_vec(vec4 inputv, int dir) {
    vec4 rev;
    if(dir == 1) {
        rev[0] = inputv[2];
        rev[1] = inputv[0];
        rev[2] = inputv[1];
    } else {
        rev[0] = inputv[1];
        rev[1] = inputv[2];
        rev[2] = inputv[0];
    }
    return rev;
}

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
    vec4 color3 = texture(samp, tc/2);
    vec4 color4 = texture(mat_samp, tc/2);
    vec2 st = (gl_FragCoord.xy / iResolution_.xy) + timeval;
    float r = random(st);
    int x1 = 0, x2 = 0, x3 = 0;
    if(r < 0.3) {
        x1 = 0;
        x2 = 1;
        x3 = 2;
    } else if(r > 0.3 && r < 0.7){
        x1 = 1;
        x2 = 2;
        x3 = 0;
    } else {
        x1 = 2;
        x2 = 0;
        x3 = 1;
    }
    if(int(alpha_r)%2 == 0) {
        color[0] = (color[0] * 0.5) + (color2[x1] * 0.5);
        color[1] = (color[1] * 0.5) + (color3[x2] * 0.5);
        color[2] = (color[2] * 0.5) + (color4[x3] * 0.5);
    } else {
        color[x1] = (color[0] * 0.5) + (color2[0] * 0.5);
        color[x2] = (color[1] * 0.5) + (color3[1] * 0.5);
        color[x3] = (color[2] * 0.5) + (color4[2] * 0.5);
        
    }
}






