#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

void main(void) {
    vec2 normCoord = gl_FragCoord.xy / iResolution.xy;
    float distortionStrength = 0.2;
    float noiseFactor = sin(normCoord.x * 10.0 + time_f) * cos(normCoord.y * 10.0 + time_f);
        vec2 distortedCoord = normCoord + distortionStrength * vec2(noiseFactor, noiseFactor);

    distortedCoord = clamp(distortedCoord, 0.0, 1.0);
    color = texture(samp, distortedCoord);
}
