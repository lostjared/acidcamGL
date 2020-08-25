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
uniform vec2 mat_size;
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
    vec4 color2 = texture(mat_samp, tc);
    
    vec2 st = (gl_FragCoord.xy / iResolution_.xy) + timeval;

    float rnd = random(st);

    vec4 color1 = vec4(0.2, 0.4, 0.8, 1.0);
    
    color1 = texture(mat_samp, tc);
    
    float x_pos = gl_FragCoord.x/2;
    float y_pos = gl_FragCoord.y/2;
 
    if(int(x_pos)%2 == 0) {
        color = color * color1 * alpha;
    } else if(int(y_pos)%2 == 0) {
        color = color * color1.bgra * alpha;
    } else {
        color = color * color1 * fract(timeval/3)*6.0 * alpha;
    }
}
