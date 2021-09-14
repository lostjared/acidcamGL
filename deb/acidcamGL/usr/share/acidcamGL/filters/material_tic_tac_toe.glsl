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

    int x = int(gl_FragCoord.x);
    int y = int(gl_FragCoord.y);
    int w = int(iResolution_.x);
    int h = int(iResolution_.y);
    int cx = int(inc_valuex.x);
    int cy = int(inc_valuex.x);
    
    int valuex = x/cx;
    int valuey = y/cy;
    
    vec4 value = vec4(0.2, 0.4, 0.6, 1.0);
    
    if(x%cx==0) {
        //color = vec4(1,1,1,1);
        color = color * color2 * value;
    } else if(y%cy==0) {
        color = color * color2 * value;
    } else {
        if(valuex%2==0)
            color = color * color2 * value;
        else if(valuey%2==0)
            color = color * color2 * random_value/255 * alpha;
        else
            color = color * color2 * value;
    }

    color *= 4.0;
}
