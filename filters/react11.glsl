#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;

void main(void) {
    float amplitude = sin(time_f * 5.0) * 2.0;
    vec2 center = vec2(0.5, 0.5);
    vec2 direction = tc - center;
    float distance = length(direction);
    float angle = atan(direction.y, direction.x);
    float spin = amplitude * distance;

    float newAngle1 = angle + spin;
    float newAngle2 = angle - spin;
    
    vec2 distorted_tc1 = center + vec2(cos(newAngle1), sin(newAngle1)) * distance;
    vec2 distorted_tc2 = center + vec2(cos(newAngle2), sin(newAngle2)) * distance;
    
    distorted_tc1 = clamp(distorted_tc1, vec2(0.0), vec2(1.0));
    distorted_tc2 = clamp(distorted_tc2, vec2(0.0), vec2(1.0));

    vec4 color1 = texture(samp, distorted_tc1);
    vec4 color2 = texture(samp, distorted_tc2);

    color = mix(color1, color2, 0.5);
}
