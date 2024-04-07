// matrix
#version 330 core
out vec4 color;
in vec2 tc;
uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

void main(void) {
    float time_t = mod(time_f, 10.0);
    vec2 normCoord = (tc * 2.0 - 1.0) * vec2(iResolution.x / iResolution.y, 1.0);
    float refractionIndex = time_t * 0.5;
    float radius = 1.0 * tan(time_t);
    float distSquared = dot(normCoord, normCoord);
    normCoord *= 1.0 + refractionIndex * distSquared;
    vec2 coord = tan(normCoord * time_t) / vec2(iResolution.x / iResolution.y, 1.0) * 0.5 + 0.5;
    color = texture(samp, coord);
}
