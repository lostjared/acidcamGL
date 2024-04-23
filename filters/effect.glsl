#version 330 core
out vec4 color;
in vec2 tc;
uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;
float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}
void main(void) {
    vec2 uv = tc;
    vec2 warp = uv + vec2(
        sin(uv.y * 10.0 + time_f) * 0.1,
        sin(uv.x * 10.0 + time_f) * 0.1
    );
    vec3 colorShift = vec3(
        0.5 * sin(time_f * 0.5) + 0.5,
        0.5 * sin(time_f * 0.7 + 2.0) + 0.5,
        0.5 * sin(time_f * 0.3 + 4.0) + 0.5
    );
    float feedback = rand(uv + time_f);
    vec2 feedbackUv = warp + feedback * 0.01;
    vec4 texColor = texture(samp, feedbackUv);
    vec3 finalColor = texColor.rgb + colorShift;
    color = sin(vec4(finalColor, texColor.a) * (time_f * 0.5));
}
