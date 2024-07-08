#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;

void main(void) {
    vec2 center = vec2(0.5, 0.5);
    vec2 uv = tc - center;
    float dist = length(uv);
    float angle = time_f * 2.0 + dist * 5.0;
    float s = sin(angle);
    float c = cos(angle);

    uv = vec2(
        uv.x * c - uv.y * s,
        uv.x * s + uv.y * c
    );
    
    uv += center;

    color = texture(samp, uv);
}
