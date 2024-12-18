#version 330 core

out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}


void main(void) {
    vec2 uv = tc;
    float maxTime = pingPong(time_f, 10.0);
    float time = pingPong(time_f, maxTime);
    float gForceIntensity = 0.1;
    float waveFrequency = 10.0;
    float waveAmplitude = 0.03;
    float gravityPull = gForceIntensity * time * uv.y;
    float waveDistortion = waveAmplitude * sin(waveFrequency * uv.x + time * 2.0);
    vec2 centeredUV = uv - vec2(0.5, 0.5);
    float angle = time + length(centeredUV) * 4.0;
    float spiralAmount = 0.5 * (1.0 - uv.y);
    mat2 rotation = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
    vec2 spiraledUV = rotation * centeredUV * (1.0 + spiralAmount);
    vec2 distortedUV = spiraledUV + vec2(0.5, 0.5);
    distortedUV.y += gravityPull + waveDistortion;
    distortedUV = clamp(distortedUV, vec2(0.0), vec2(1.0));
    color = texture(samp, distortedUV);
}
