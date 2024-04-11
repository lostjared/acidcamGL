#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

float ripple(vec2 pos, float time, float speed, float frequency) {
    float aspectRatio = iResolution.x / iResolution.y;
    pos.x *= aspectRatio;
    float d = distance(pos, vec2(0.5 * aspectRatio, 0.5));
    return sin(d * frequency - time * speed) * exp(-d * 3.0);
}

void main(void) {
    vec2 pos = tc;
    float time_t = mod(time_f, 3.5);
    float x = ripple(pos, time_f, 0.5, 12.0);
    pos = (pos * x) + (pos * time_t);
    color = texture(samp, pos);
}

