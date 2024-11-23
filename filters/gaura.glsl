#version 330 core

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

out vec4 color;

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}


void main() {
    vec2 uv = gl_FragCoord.xy / iResolution;
    vec4 texColor = texture(samp, uv);

    float pulse = 0.5 + 0.5 * sin(time_f * 3.0);

    vec2 offset1 = vec2(0.01, 0.02) * sin(time_f + length(uv - 0.5) * 20.0);
    vec2 offset2 = vec2(-0.02, 0.01) * cos(time_f * 1.5 + length(uv - 0.5) * 15.0);
    vec2 offset3 = vec2(0.03, -0.03) * sin(time_f * 5.0);

    vec3 aura1 = texture(samp, uv + offset1).rgb * 0.6;
    vec3 aura2 = texture(samp, uv + offset2).rgb * 0.4;
    vec3 aura3 = texture(samp, uv + offset3).rgb * 0.3;

    vec3 strobingGradient = vec3(
        0.5 + 0.5 * sin(time_f + uv.x * 10.0),
        0.5 + 0.5 * cos(time_f + uv.y * 10.0),
        0.5 + 0.5 * sin(time_f + (uv.x + uv.y) * 10.0)
    );

    vec3 auraGlow = mix(aura1 + aura2 + aura3, strobingGradient, 0.5);
    float auraStrength = 0.4 + 0.3 * sin(time_f * 4.0 + length(uv - 0.5) * 10.0);

    vec3 finalAura = auraGlow * auraStrength;
    color = vec4(sin(finalAura * (pingPong(time_f, 8.0) + 2.0)), 0.5 * texColor.a);
}

