#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform sampler2D mat_samp;
uniform float time_f;
uniform vec2 iResolution;


void main(void) {
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    float timeBasedBlockSize = 2.0 + 62.0 * abs(sin(time_f));
    int blockSize = int(timeBasedBlockSize);

    ivec2 blockOrigin = ivec2(uv * iResolution) / blockSize * blockSize;

    vec3 currentPixel = texture(samp, blockOrigin / iResolution).rgb;
    vec3 previousPixel = texture(mat_samp, blockOrigin / iResolution).rgb;

    vec3 mixedPixel = mix(currentPixel, previousPixel, 0.5);
    color = vec4(mixedPixel, 1.0);
}
