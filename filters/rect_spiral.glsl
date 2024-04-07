#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    float loop_dur = 20.0;
    float current_t = mod(time_f, loop_dur);
    vec2 uv = (tc - 0.5) * (iResolution.x / iResolution.y, 1.0);
    float radius = length(uv);
    float angle = atan(uv.y, uv.x);
    float spiralSpeed = 1.0;
    angle += current_t * spiralSpeed;
    radius *= current_t;     vec2 spiralCoord = vec2(cos(angle), sin(angle)) * radius;

    vec2 transformedCoord = sin(spiralCoord * radius/2);
    
    vec2 finalCoord = transformedCoord * 0.5 + 0.5;
    color = texture(samp, finalCoord);
}
