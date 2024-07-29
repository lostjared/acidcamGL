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
    
    float time_t = pingPong(time_f, 10.0) + 2.0;
    
    vec2 uv = tc;
    float x = uv.x + sin(uv.y * 10.0 + time_f * 5.0) * 0.05;
    float y = uv.y + cos(uv.x * 10.0 + time_f * 5.0) * 0.05;
    vec2 displacedUV = vec2(x, y);
    vec4 texColor = texture(samp, displacedUV);
    float haze = sin(uv.y * 20.0 + time_f * 10.0) * 0.5 + 0.5;
    vec4 hazeColor = vec4(0.8, 0.3, 1.0, 1.0) * haze;
    color = sin((texColor + hazeColor)*time_t);
}
