#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    float t = mod(time_f, 10.0);
    float scale = pow(1.1, t);

    vec2 centeredTC = (tc - 0.5) * scale + 0.5;
    
    float xDistort = cos(centeredTC.y * 10.0 + time_f) * 0.1 / scale;
    float yDistort = sin(centeredTC.x * 10.0 + time_f) * 0.1 / scale;
    float tanDistortX = tan(centeredTC.x * 5.0 + time_f) * 0.05 / scale;
    float tanDistortY = tan(centeredTC.y * 5.0 + time_f) * 0.05 / scale;

    vec2 distortedTC = centeredTC + vec2(xDistort + tanDistortX, yDistort + tanDistortY);
    distortedTC = fract(distortedTC);

    color = texture(samp, distortedTC);
}
