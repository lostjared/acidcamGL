#version 410
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
uniform vec2 iResolution;

uniform float restore_black;
in float restore_black_value;
in vec2 iResolution_;

void main(void)
{
    if(restore_black_value == 1.0 && texture(samp, tc) == vec4(0, 0, 0, 1))
        discard;
    color = texture(samp, tc);
    vec4 back_color = color;
    float x = gl_FragCoord.x / 16;
    float y = gl_FragCoord.y / 16;
    color[0] += (color[0] * 255 / x) / 255;
    color[1] = 0;
    color[2] += (color[2] * 255 / y) / 255;

    color[0] = color[0] * 0.8;
    color[2] = color[2] * 0.8;
}

