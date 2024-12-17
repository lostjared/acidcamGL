#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

vec2 rotate(vec2 pos, float angle) {
    float s = sin(angle);
    float c = cos(angle);
    pos -= vec2(0.5);
    pos = mat2(c, -s, s, c) * pos;
    pos += vec2(0.5);
    return pos;
}

void main(void) {
    vec2 pos = tc;
    float aspectRatio = iResolution.x / iResolution.y;
    pos.x *= aspectRatio;

    float spinSpeed = time_f * 0.5;
    pos = rotate(pos, spinSpeed);
    float dist = distance(pos, vec2(0.5 * aspectRatio, 0.5));
    float scale = 1.0 + 0.2 * sin(dist * 15.0 - time_f * 2.0);
    pos = (pos - vec2(0.5)) * scale + vec2(0.5);
    color = texture(samp, pos);
}

