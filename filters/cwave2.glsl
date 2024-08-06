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
    vec2 uv = tc * iResolution;
    uv -= 0.5 * iResolution;
    float dist = length(uv);
    float angle = atan(uv.y, uv.x);
    float time_t = pingPong(time_f, 20.0);
    float wave = sin(dist * 10.0 - time_f * 5.0 + angle * 5.0);
    wave = sin(wave * time_t);
    
    vec4 texColor = texture(samp, tc);
    
    color = texColor * (0.5 + 0.5 * wave);
}

