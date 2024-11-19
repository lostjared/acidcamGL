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

void main(void) {
    vec2 uv = tc;
    uv.x += sin(uv.y * 10.0 + time_f) * 0.1;
    uv.y += cos(uv.x * 10.0 + time_f) * 0.1;
    float modFactor = pingPong(time_f * 0.5, 1.0);
    vec4 texColor = texture(samp, uv);
    vec3 psychedelicColor = vec3(
        texColor.r * sin(time_f + uv.y * 10.0),
        texColor.g * cos(time_f + uv.x * 10.0),
        texColor.b * sin(time_f + uv.y * 5.0)
    );
    psychedelicColor = mix(texColor.rgb, psychedelicColor, modFactor);
    float time_t =  pingPong(time_f, 10.0) + 2.0;
    color = vec4(sin(psychedelicColor * time_t), texColor.a);
}

