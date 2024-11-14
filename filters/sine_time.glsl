#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

void main(void) {
    vec4 color_solid = vec4(0.3, 0.1, 0.8, 1.0);
    float time_t = pingPong(time_f, 10.0) + 2.0;
    vec4 ctx = texture(samp, tc);
    color = vec4(sin(ctx.rgb * time_t) , ctx.a);
    color = mix(color, color_solid, 0.5);
}
