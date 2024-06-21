#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;

void main(void) {
    vec4 texColor = texture(samp, tc);
    float sparkle = abs(sin(time_f * 10.0 + tc.x * 100.0) * cos(time_f * 15.0 + tc.y * 100.0));
    vec3 magicalColor = vec3(sin(time_f * 2.0) * 0.5 + 0.5, cos(time_f * 3.0) * 0.5 + 0.5, sin(time_f * 4.0) * 0.5 + 0.5);
    color = texColor + vec4(magicalColor * sparkle, 1.0);
}
