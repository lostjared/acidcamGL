#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;
uniform vec4 random_var;

vec4 blendColors(float t) {
    vec4 color1 = vec4(0.6, 0.2, 0.8, 1.0);
    vec4 color2 = vec4(0.2, 0.8, 0.6, 1.0);
    vec4 color3 = vec4(0.8, 0.6, 0.2, 1.0);
    vec4 color4 = vec4(0.3, 0.4, 0.6, 1.0);
    float phase = mod(t / 20.0, 2.0 * 3.14159);
    float blend = (sin(phase) + 1.0) / 2.0;
    if (t < 25.0)
        return mix(color1, color2, blend);
    else if (t < 50.0)
        return mix(color2, color3, blend);
    else
        return mix(color3, color4, blend);
}

void main(void) {
    color = texture(samp, tc);
    float time_t = mod(time_f, 75.0);
    vec4 light = blendColors(time_t);
    color = tan(0.25 + sin((color * light) * time_t));
}
