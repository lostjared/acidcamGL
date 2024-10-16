#version 330
in vec2 tc;
out vec4 color;
uniform float time_f;
uniform sampler2D samp;
uniform vec2 iResolution;

void main(void) {
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    vec2 reflectedUV = vec2(1.0 - uv.x, uv.y);
    float noise = sin(reflectedUV.y * 50.0 + time_f * 5.0) * 0.01;
    float distortionX = noise;
    float distortionY = cos(reflectedUV.x * 50.0 + time_f * 5.0) * 0.01;
    vec2 distortion = vec2(distortionX, distortionY);
    float r = texture(samp, reflectedUV + distortion * 1.2).r;
    float g = texture(samp, reflectedUV + distortion * 0.8).g;
    float b = texture(samp, reflectedUV + distortion * 0.4).b;
    color = vec4(r, g, b, 1.0);
}
