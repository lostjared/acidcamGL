#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

float ripple(vec2 pos, float time, float speed, float frequency) {
    float aspectRatio = iResolution.x / iResolution.y;
    pos.x *= aspectRatio;
    float d = distance(pos, vec2(0.5 * aspectRatio, 0.5));
    return sin(d * frequency - time * speed) * exp(-d * 3.0);
}

vec3 psychedelicColors(float value) {
    return 0.5 + 0.5 * cos(6.2831 * vec3(0.0, 0.33, 0.66) + value);
}

void main(void) {
    vec2 pos = tc - 0.5;
    pos.y *= iResolution.y / iResolution.x;
    float rippleDist = ripple(pos, time_f, 0.4, 10.0);
    vec2 distortedPos = pos + 0.05 * vec2(cos(rippleDist * 3.0), sin(rippleDist * 2.0));
    distortedPos += 0.1 * vec2(sin(time_f * 0.8), cos(time_f * 0.8));
    vec4 texColor = texture(samp, distortedPos + 0.5);
    vec3 colorCycle = psychedelicColors(time_f + rippleDist);
    color = vec4(texColor.rgb * colorCycle, texColor.a);
}

