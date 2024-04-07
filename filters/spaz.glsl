#version 330 core
out vec4 color;
in vec2 tc;
uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;
void main(void) {
    vec2 normCoord = (tc * 2.0 - 1.0);
    float distortionX = sin(normCoord.y * 10.0 + time_f * 3.0) * 0.05;
    float distortionY = cos(normCoord.x * 15.0 + time_f * 2.0) * 0.05;
    distortionX += sin(time_f * 5.0) * 0.02;
    distortionY += cos(time_f * 7.0) * 0.02;
    vec2 distortedCoord = normCoord + vec2(distortionX, distortionY);
    distortedCoord = (distortedCoord + 1.0) / 2.0;
    distortedCoord = clamp(distortedCoord, 0.0, 1.0);
    color = texture(samp, distortedCoord);
}
