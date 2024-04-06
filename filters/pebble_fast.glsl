#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;
void main(void) {
    vec2 normCoord = (tc * 2.0 - 1.0) * vec2(iResolution.x / iResolution.y, 1.0);

    float dist = length(normCoord);
    float maxRippleRadius = 25.0;
    float rippleSpeed = 2.0;
    float phase = mod(time_f * rippleSpeed, maxRippleRadius);
    
    float ripple = sin((dist - phase) * 10.0) * exp(-dist * 3.0);
    vec2 displacedCoord = vec2(tc.x, tc.y + ripple * sin(time_f));
    color = texture(samp, displacedCoord);
}
