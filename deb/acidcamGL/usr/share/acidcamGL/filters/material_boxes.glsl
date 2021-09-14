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
uniform vec4 inc_value;
uniform vec4 inc_valuex;
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
    vec4 orig_color = color;
    vec4 color2 = texture(mat_samp, tc);
    
    float pos = gl_FragCoord.x / inc_valuex.x;
    float pos2 = gl_FragCoord.y / inc_value.y;
    float f = fract(pos);
    f *= 3;
    float f2 = fract(pos2);
    f2 *= 2;
    color = color * (sin(f)*alpha) * (tan(f2)*alpha) * color2;
    if(color.r < 0.1 && color.g < 0.1 && color.b < 0.1)
        color = orig_color;
}





