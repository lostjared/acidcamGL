#version 330 core

out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    float N = 8.0;
    vec2 center = vec2(0.5, 0.5);
    vec2 pos = tc - center;
    float angle = atan(pos.y, pos.x);
    float radius = length(pos);
    float speed = time_f * 0.05;
    angle += speed;
    angle = mod(angle, 6.2831853 / N);
    angle = abs(angle - (3.14159265 / N));
    vec2 kaleidoscopicTC;
    kaleidoscopicTC.x = radius * cos(angle);
    kaleidoscopicTC.y = radius * sin(angle);
    kaleidoscopicTC += center;
    vec2 warpedCoords;
    warpedCoords.x = fract(kaleidoscopicTC.x + time_f * 1.0);
    warpedCoords.y = fract(kaleidoscopicTC.y + time_f * 1.0);
    color = texture(samp, warpedCoords);
}
