#version 330 core
out vec4 color;
in vec2 tc;
uniform sampler2D samp;
uniform sampler2D mat_samp;
uniform vec4 optx;
uniform float time_f;
uniform vec2 iResolution;
uniform float value_alpha_r, value_alpha_g, value_alpha_b;
void main() {
    vec2 uv = tc;
    vec4 currentColor = texture(samp, uv);
    vec4 pastColor = texture(samp, uv * optx.xy);
    float mixFactor = abs(sin(time_f * value_alpha_r));
    vec3 resultColor = mix(currentColor.rgb, pastColor.rgb, step(0.5, mixFactor));
    resultColor = mix(resultColor, vec3(1.0) - resultColor, step(0.5, mod(time_f, 2.0)));
    color = vec4(resultColor, 1.0);
    color = mix(color, texture(mat_samp, tc), 0.5);
}
