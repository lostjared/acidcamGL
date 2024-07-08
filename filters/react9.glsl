#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;

vec3 getRainbowColor(float t) {
    float r = 0.5 + 0.5 * cos(6.28318 * (t + 0.0));
    float g = 0.5 + 0.5 * cos(6.28318 * (t + 0.333));
    float b = 0.5 + 0.5 * cos(6.28318 * (t + 0.666));
    return vec3(r, g, b);
}

void main(void) {
    vec2 redOffset = vec2(sin(time_f * 5.0), cos(time_f * 5.0)) * 0.02;
    vec2 greenOffset = vec2(cos(time_f * 7.0), sin(time_f * 7.0)) * 0.02;
    vec2 blueOffset = vec2(sin(time_f * 3.0), cos(time_f * 3.0)) * 0.02;

    vec4 redChannel = texture(samp, tc + redOffset);
    vec4 greenChannel = texture(samp, tc + greenOffset);
    vec4 blueChannel = texture(samp, tc + blueOffset);

    vec4 originalColor = vec4(redChannel.r, greenChannel.g, blueChannel.b, 1.0);
    vec3 rainbowColor = getRainbowColor(time_f * 0.1);
    vec3 mixedColor = mix(originalColor.rgb, rainbowColor, 0.5);

    float beat = abs(sin(time_f * 10.0));

    color = vec4(mix(originalColor.rgb, mixedColor, beat), 1.0);
}
