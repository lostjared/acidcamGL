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

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

void main(void)
{
    float time_x = pingPong(time_f, 2.0) + 1.0;
    float time_y = pingPong(time_f, 4.0) + 1.0;
    float time_z = pingPong(time_f, 8.0) + 1.0;
    color = texture(samp, tc);
    vec4 color2 = texture(samp, tc /time_x);
    vec4 color3 = texture(samp, tc/time_y);
    vec4 color4 = texture(samp, tc/time_z);
    color = (color * 0.4) + (color2 * 0.4) + (color3 * 0.4) + (color4 * 0.4) ;
    float time_q = pingPong(time_f, 25.0);
    color = cos(sin(length(time_q) * color));
}



