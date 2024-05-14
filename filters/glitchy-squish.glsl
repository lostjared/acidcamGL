#version 330 core

in vec2 tc;
out vec4 color;

uniform sampler2D samp;
uniform float time_f;

void main(void)
{
    vec2 uv = tc * 2.0 - 1.0;
    float t = time_f * 0.5;

    // Glitch effect
    float glitchX = sin(uv.y * 10.0 + t) * 0.1;
    float glitchY = cos(uv.x * 10.0 + t) * 0.1;
    vec2 glitchUV = vec2(uv.x + glitchX, uv.y + glitchY);

    // Psychedelic color manipulation
    float r = length(glitchUV);
    float a = atan(glitchUV.y, glitchUV.x);
    float radius = sin(t + r * 5.0) * 0.5 + 0.5;
    float angle = a + t + sin(r * 20.0 + t) * 0.1;
    vec2 distorted_uv = vec2(cos(angle), sin(angle)) * radius + 0.5;

    vec3 col = texture(samp, distorted_uv).rgb;
    
    col = mix(col, vec3(1.0, 0.0, 0.0), 0.5 * sin(t + length(uv) * 5.0)); // Add psychedelic color effect

    color = vec4(col, 1.0);
}
