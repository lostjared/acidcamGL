
#version 410
in vec2 tc;
out vec4 color;
in float alpha_r;
in float alpha_g;
in float alpha_b;
in float current_index;
in float timeval;
in float alpha;
out vec3 vpos;
uniform float alpha_value;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform sampler2D samp;
uniform float value_alpha_r, value_alpha_g, value_alpha_b;
uniform float index_value;
uniform float time_f;

void main(void)
{
    color = texture(samp, tc);
    color[0] += timeval*cos(0.1489*tc[0]);
    color[1] += timeval*sin(0.1482*tc[1]);
    ivec3 int_color;
    for(int i = 0; i < 3; ++i) {
        int_color[i] = int(255 * color[i]);
        int_color[i] = int_color[i]%255;
        color[i] = float(int_color[i])/255;
    }
}

