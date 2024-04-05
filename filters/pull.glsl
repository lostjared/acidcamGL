#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

void main(void) {
    vec2 centeredCoord = (tc * 2.0) - vec2(1.0, 1.0);
    float stretchFactor = 1.0 + (1.0 - abs(centeredCoord.y)) * 0.5;
    centeredCoord.x *= sin(stretchFactor * time_f);
    vec2 stretchedCoord = (centeredCoord + vec2(1.0, 1.0)) / 2.0;
    color = texture(samp, stretchedCoord);
}
