#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

vec2 pseudoRandomDirection(float time) {
    return normalize(vec2(sin(time * 1.3), cos(time * 1.7)));
}

void main(void) {
    vec2 centeredCoord = (tc * 2.0 - 1.0) * vec2(iResolution.x / iResolution.y, 1.0);

    float dist = length(centeredCoord);
    vec2 spiralDir = pseudoRandomDirection(time_f + dist * 5.0);     float angle = atan(centeredCoord.y, centeredCoord.x) + time_f * 2.0;
    float radius = dist * (1.0 + 0.1 * sin(time_f * 3.0 + dist * 10.0));
    vec2 spiralCoord = vec2(cos(angle), sin(angle)) * radius * 0.5 + spiralDir * 0.1 * sin(time_f * 2.0);
    
    spiralCoord = (spiralCoord / vec2(iResolution.x / iResolution.y, 1.0) + 1.0) / 2.0;

    color = texture(samp, spiralCoord);
}

