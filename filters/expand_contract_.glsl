#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

void main(void) {
    vec2 normCoord = (tc * 2.0) - vec2(1.0, 1.0);
    normCoord.x *= iResolution.x / iResolution.y;
    float dist = length(normCoord);

    float stretchMagnitude = 0.5 + sin(time_f) * 0.50;
    float stretchFactor = 1.0 + pow(dist, 2.0) * stretchMagnitude;
    vec2 stretchedCoord = normCoord / stretchFactor;
    stretchedCoord.x *= iResolution.y / iResolution.x;
    stretchedCoord = (stretchedCoord + vec2(1.0, 1.0)) / 2.0;

    stretchedCoord = clamp(stretchedCoord, 0.0, 1.0);
    color = texture(samp, stretchedCoord);
}
