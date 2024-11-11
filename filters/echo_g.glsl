#version 330
in vec2 tc;
out vec4 color;
in float restore_black_value;
uniform sampler2D samp;
uniform float time_f;


float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

void main(void) {
    float time_t = pingPong(time_f, 10.0) + 2.0;
    vec2 offset = vec2(0.01, 0.01) * time_t;
    vec4 color1 = texture(samp, tc);
    vec4 color2 = texture(samp, tc - offset);
    vec4 color3 = texture(samp, tc - offset * 2.0);
    vec4 color4 = texture(samp, tc - offset * 3.0);
    float weight1 = 0.5 * (1.0 - time_t);
    float weight2 = 0.3 * time_t;
    float weight3 = 0.15 * time_t;
    float weight4 = 0.05 * time_t;
    float totalWeight = weight1 + weight2 + weight3 + weight4;
    color = (color1 * weight1 + color2 * weight2 + color3 * weight3 + color4 * weight4) / sin(totalWeight * time_f);
    
}
