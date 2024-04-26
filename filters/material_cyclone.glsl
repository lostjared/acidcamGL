#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform sampler2D mat_samp;
uniform vec2 iResolution;
uniform float time_f;

void main(void) {
    vec2 normCoord = gl_FragCoord.xy / iResolution.xy;

    vec2 centeredCoord = normCoord - vec2(0.5, 0.5);
    centeredCoord.x *= iResolution.x / iResolution.y;
    float r = length(centeredCoord);
    float theta = atan(centeredCoord.y, centeredCoord.x);
    float twistAmount = 15.0;
    theta += (1.0 - r) * twistAmount * sin(time_f);
    vec2 twistedCoord = vec2(cos(theta), sin(theta)) * r;

    twistedCoord.x *= iResolution.y / iResolution.x;

    twistedCoord += vec2(0.5, 0.5);
    color = texture(samp, twistedCoord);
    vec4 color2 = texture(mat_samp, twistedCoord);
    color = mix(color, color2, 0.5);
}

