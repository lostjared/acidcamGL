#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

vec2 twist(vec2 coord, float twistAmount, float time, float direction) {
    float r = length(coord);
    float theta = atan(coord.y, coord.x);
    theta += (1.0 - r) * twistAmount * sin(time * direction);
    return vec2(cos(theta), sin(theta)) * r;
}

void main(void) {
    vec2 normCoord = gl_FragCoord.xy / iResolution.xy;

    vec2 centeredCoord = normCoord - vec2(0.5, 0.5);
    centeredCoord.x *= iResolution.x / iResolution.y;

    vec2 twistClockwise = twist(centeredCoord, 15.0, time_f, 1.0);

    vec2 twistCounterClockwise = twist(centeredCoord, 15.0, time_f, -1.0);

    twistClockwise.x *= iResolution.y / iResolution.x;
    twistCounterClockwise.x *= iResolution.y / iResolution.x;

    twistClockwise += vec2(0.5, 0.5);
    twistCounterClockwise += vec2(0.5, 0.5);
    vec4 texColorClockwise = texture(samp, twistClockwise);
    vec4 texColorCounterClockwise = texture(samp, twistCounterClockwise);

    color = mix(texColorClockwise, texColorCounterClockwise, 0.5);
}
