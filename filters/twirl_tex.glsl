#version 330
in vec2 tc;
out vec4 color;
uniform float time_f;
uniform sampler2D samp;
uniform vec2 iResolution;

void main(void) {
    vec2 center = vec2(0.5, 0.5);
    vec2 normCoord = tc - center;
    normCoord.x *= iResolution.x / iResolution.y;
    float distance = length(normCoord);
    float angle = atan(normCoord.y, normCoord.x);
    float twirlAmount = 5.0;
    angle += distance * twirlAmount * sin(time_f);
    vec2 twirledCoord = vec2(cos(angle), sin(angle)) * distance;
    twirledCoord.x *= iResolution.y / iResolution.x;
    color = texture(samp, twirledCoord + center);
}
