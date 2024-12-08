#version 330 core

out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;
uniform float alpha;

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

void main(void) {
    vec2 uv = (tc * iResolution - 0.5 * iResolution) / iResolution.y;

    float t = time_f * 0.7;

    float radius = length(uv);
    float angle = atan(uv.y, uv.x);
    float swirlAmount = sin(t) * 2.0;
    angle += swirlAmount * exp(-radius * 4.0);

    uv = vec2(cos(angle), sin(angle)) * radius;

    float wave = sin(radius * 10.0 + t * 6.0) * 0.05;
    uv += vec2(cos(angle), sin(angle)) * wave;

    vec3 texColor = texture(samp, uv * 0.5 + 0.5).rgb;

    float modAngle = angle + t * 0.5;
    vec3 swirlColor = vec3(
        sin(modAngle * 3.0),
        sin(modAngle * 2.0 + 1.0),
        sin(modAngle * 1.0 + 2.0)
    ) * 0.5 + 0.5;

    vec3 finalColor = mix(swirlColor, texColor, 0.7);

    float pulse = abs(sin(time_f * 3.14159)) * 0.2 + 0.8;
    finalColor *= pulse;

    color = vec4(finalColor, alpha);
}
