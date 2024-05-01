#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

void main(void) {
    vec2 centeredCoord = (tc * 2.0 - 1.0) * vec2(iResolution.x / iResolution.y, 1.0);
    float angle = atan(centeredCoord.y, centeredCoord.x) + time_f;
    float radius = length(centeredCoord);
    float spiral = sin(10.0 * angle - 3.0 * time_f) * exp(-3.0 * radius);
    vec3 lightColor = vec3(0.1, 0.5, 0.8) * 0.5 * (1.0 + spiral);
    lightColor = sin(lightColor * time_f);
    vec4 texColor = texture(samp, tc);
    color = vec4(texColor.rgb * lightColor, texColor.a);
}
