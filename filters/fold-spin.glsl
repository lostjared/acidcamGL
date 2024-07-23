#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec2 uv = tc * 2.0 - 1.0;
    float angle = atan(uv.y, uv.x);
    float radius = length(uv);

    float spiralTime = mod(time_f, 4.0);
    float maxRadius = sqrt(2.0); 
    if (spiralTime < 2.0) {
        radius = mix(radius, maxRadius, spiralTime / 2.0);
    } else {
        radius = mix(maxRadius, radius, (spiralTime - 2.0) / 2.0);
    }
    angle += spiralTime * 3.14159;

    uv = vec2(cos(angle), sin(angle)) * radius;
    uv = (uv + 1.0) / 2.0;

    color = texture(samp, uv);
}
