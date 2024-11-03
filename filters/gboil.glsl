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

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(void) {
    vec2 uv = tc;
    float frequency = 30.0;
    float amplitude = 0.05;
    float speed = 5.0;
    float t = time_f * speed;
    float noiseX = rand(uv * frequency + t);
    float noiseY = rand(uv * frequency - t);
    float distortionX = amplitude * pingPong(t + uv.y * frequency + noiseX, 1.0);
    float distortionY = amplitude * pingPong(t + uv.x * frequency + noiseY, 1.0);
    vec2 tc_mod = uv + vec2(distortionX, distortionY);
    color = texture(samp, tc_mod);
}
