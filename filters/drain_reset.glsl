#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

void main(void) {
    float loopDuration = 5.0;
    float currentTime = mod(time_f, loopDuration);
    vec2 normCoord = (tc * 2.0 - 1.0) * vec2(iResolution.x / iResolution.y, 1.0);
    float dist = length(normCoord);
    float angle = atan(normCoord.y, normCoord.x);
    float spiralSpeed = 5.0;
    float inwardSpeed = currentTime / loopDuration;     angle += (1.0 - smoothstep(0.0, 8.0, dist)) * currentTime * spiralSpeed;
    dist *= 1.0 - inwardSpeed;
    vec2 spiralCoord = vec2(cos(angle), sin(angle)) * tan(dist);
    spiralCoord = (spiralCoord / vec2(iResolution.x / iResolution.y, 1.0) + 1.0) / 2.0;
    color = texture(samp, spiralCoord);
}
