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
    float time_t = pingPong(time_f, 7.0);
    vec3 neonPurple = vec3(199.0/255.0, 0.0, 1.0);
    vec3 neonPink   = vec3(1.0, 0.0, 127.0/255.0);
    vec3 gradientColor = mix(neonPink, neonPurple, tc.y);
    color = mix(vec4(sin(gradientColor * time_t), 1.0), texture(samp, tc), 0.5);
}
