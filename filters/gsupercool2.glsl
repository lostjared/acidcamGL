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

    float angle = atan(uv.y - 0.5, uv.x - 0.5);
    float modulatedTime = pingPong(time_f, 5.0);
    angle += modulatedTime;

    vec2 rotatedTC;
    rotatedTC.x = cos(angle) * (uv.x - 0.5) - sin(angle) * (uv.y - 0.5) + 0.5;
    rotatedTC.y = sin(angle) * (uv.x - 0.5) + cos(angle) * (uv.y - 0.5) + 0.5;

    vec2 flow;
    flow.x = sin((rotatedTC.x + time_f) * 10.0) * 0.02;
    flow.y = cos((rotatedTC.y + time_f) * 10.0) * 0.02;
    vec2 uv_flow = rotatedTC + flow;

    float frequency = 30.0;
    float amplitude = 0.05;
    float speed = 5.0;
    float t = time_f * speed;
    float noiseX = rand(uv_flow * frequency + t);
    float noiseY = rand(uv_flow * frequency - t);
    float distortionX = amplitude * pingPong(t + uv_flow.y * frequency + noiseX, 1.0);
    float distortionY = amplitude * pingPong(t + uv_flow.x * frequency + noiseY, 1.0);
    vec2 uv_distorted = uv_flow + vec2(distortionX, distortionY);

    float zoom = sin(time_f * 0.5) * 0.2 + 1.0;
    vec2 uv_zoom = (uv_distorted - vec2(0.5)) * zoom + vec2(0.5);

    vec4 color_sample = texture(samp, uv_zoom);

    vec3 color_effect;
    color_effect.r = sin(color_sample.r * 10.0 + time_f);
    color_effect.g = sin(color_sample.g * 10.0 + time_f + 2.0);
    color_effect.b = sin(color_sample.b * 10.0 + time_f + 4.0);

    color = vec4(color_effect, color_sample.a);
}
