#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

void main(void) {
     vec2 normCoord = gl_FragCoord.xy / iResolution.xy;
   vec2 centeredCoord = normCoord - vec2(0.5, 0.5);
    centeredCoord.x *= iResolution.x / iResolution.y;
    float scale = 1.0 + 0.1 * sin(time_f * 2.0);
    vec2 scaledCoord = centeredCoord * scale;
    scaledCoord.x *= iResolution.y / iResolution.x;
    scaledCoord += vec2(0.5, 0.5);
    scaledCoord = clamp(scaledCoord, 0.0, 1.0);
    color = texture(samp, scaledCoord);
}
