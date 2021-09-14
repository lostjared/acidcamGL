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
    //vec4 value = vec4(0.2, 0.1, 0.8, 1.0);
    vec2 color_value1 = gl_FragCoord.xy / iResolution_.xy;

    vec4 col = vec4(0.2, 0.4, 0.8, 1.0);
    
    float rnd = random(color_value1);
    
    vec4 value = vec4(gl_FragCoord.x/255, gl_FragCoord.y/255, gl_FragCoord.x/255/rnd,1.0);
    
    col += value + alpha_r;
    color = color * col;
    color = color / 3.0;
}



