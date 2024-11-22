#version 330 core

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

out vec4 color;

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution;
    vec4 texColor = texture(samp, uv);
    float xor_effect_r = mod((texColor.r * 255.0) + sin(uv.x * 20.0 + time_f) * 255.0, 256.0) / 255.0;
    float xor_effect_g = mod((texColor.g * 255.0) + cos(uv.y * 20.0 - time_f) * 255.0, 256.0) / 255.0;
    float xor_effect_b = mod((texColor.b * 255.0) + sin((uv.x + uv.y) * 20.0 + time_f) * 255.0, 256.0) / 255.0;
    vec3 finalColor = mix(texColor.rgb, vec3(xor_effect_r, xor_effect_g, xor_effect_b), 0.5);
    color = vec4(finalColor, texColor.a);
}

