#version 330 core
out vec4 color;
in vec2 tc;
uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;
void main(void){
    vec2 normCoord = tc;
    float tearLine = sin(time_f) * 0.5 + 0.5;
    float distFromTear = abs(normCoord.x - tearLine);
    float displacement = 0.202 * (1.0 - smoothstep(0.0, 0.1, distFromTear));
    normCoord.y += sin(displacement);
    color = texture(samp, normCoord);
}
