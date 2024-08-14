#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

void main(void) {
    float angle1 = atan(tc.y - 0.5, tc.x - 0.5);
    float modulatedTime1 = pingPong(time_f, 3.0);
    angle1 += modulatedTime1;

    float angle2 = atan(tc.x - 0.5, tc.y - 0.5);
    float modulatedTime2 = pingPong(time_f * 0.5, 2.5);
    angle2 += modulatedTime2;

    float angle3 = atan(tc.y - 0.5 + modulatedTime2, tc.x - 0.5 + modulatedTime1);
    float modulatedTime3 = pingPong(time_f * 1.5, 4.0);
    angle3 += modulatedTime3;

    vec2 rotatedTC;
    rotatedTC.x = cos(angle3) * (tc.x - 0.5) - sin(angle3) * (tc.y - 0.5) + 0.5;
    rotatedTC.y = sin(angle3) * (tc.x - 0.5) + cos(angle3) * (tc.y - 0.5) + 0.5;
    
    rotatedTC = sin(rotatedTC * (modulatedTime1 * modulatedTime2));

    color = texture(samp, rotatedTC);
}
