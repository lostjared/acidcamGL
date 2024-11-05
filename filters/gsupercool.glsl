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
    vec2 uv_distorted = uv + vec2(distortionX, distortionY);
    vec2 uv_kaleidoscope = uv_distorted * 2.0 - 1.0;
    uv_kaleidoscope.x *= iResolution.x / iResolution.y;
    float angle = atan(uv_kaleidoscope.y, uv_kaleidoscope.x);
    float radius = length(uv_kaleidoscope) * 1.4142;
    float segments = 8.0;
    angle = mod(angle, 6.28318 / segments);
    angle = abs(angle - 3.14159 / segments);
    uv_kaleidoscope = vec2(cos(angle), sin(angle)) * radius;
    uv_kaleidoscope = uv_kaleidoscope * 0.5 + 0.5;
    uv_kaleidoscope = clamp(uv_kaleidoscope, 0.0, 1.0);

    float time_t = pingPong(time_f * 0.5, 7.0) + 2.0;
    float pattern = cos(radius * 10.0 - time_t * 5.0);
    vec3 colorShift = vec3(
        0.5 + 0.5 * cos(pattern + time_t + 0.0),
        0.5 + 0.5 * cos(pattern + time_t + 2.094),
        0.5 + 0.5 * cos(pattern + time_t + 4.188)
    );

    vec4 texColorKaleido = texture(samp, uv_kaleidoscope);
    vec4 texColorDistorted = texture(samp, uv_distorted);

    vec3 finalColor = texColorKaleido.rgb * colorShift;
    vec4 blendedColor = mix(vec4(finalColor, texColorKaleido.a), texColorDistorted, 0.5);

    color = sin(blendedColor * time_t);
}
