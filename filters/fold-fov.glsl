#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec2 uv = tc;
    float foldAmount = sin(time_f * 3.14159);
    
    if (foldAmount > 0.0) {
        if (uv.x < 0.5) {
            uv.x = mix(uv.x, 0.5, foldAmount);
        } else {
            uv.x = mix(uv.x, 0.5, foldAmount);
        }
    } else {
        uv.x = mix(0.5 + abs(uv.x - 0.5), uv.x, abs(foldAmount));
    }
    
    vec2 translate = vec2(sin(time_f * 2.0), cos(time_f * 2.0)) * 0.25;
    uv += translate * foldAmount;

    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
        color = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        color = texture(samp, uv);
    }
}
