#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
void main(void) {
    vec2 center = vec2(0.5, 0.5);
    float strength = 0.5 + sin(time_f) * 0.5 * 8.0;
    vec2 tcFromCenter = tc - center;
    float distance = length(tcFromCenter);
    float angle = atan(tcFromCenter.y, tcFromCenter.x);
    float radius = distance * (1.0 + strength * (distance * distance));
    vec2 distortedTC = center + vec2(cos(angle), sin(angle)) * radius;
    distortedTC = clamp(distortedTC, 0.0, 1.0);
    color = texture(samp, distortedTC);
}
