#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp; // The existing texture
uniform vec2 iResolution; // The resolution of the viewport
uniform float time_f; // Uniform to control animation, typically the elapsed time

// A simple noise function
float noise(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void main(void) {
    vec2 uv = tc;

    // Create a time-based moving effect
    float time = time_f * 0.1;

    // Get a noisy offset
    vec2 noiseOffset = vec2(noise(uv + time), noise(uv - time));
    noiseOffset = (noiseOffset - 0.5) * 0.2; // Adjust range and scale of noise

    // Apply noise offset to texture coordinates
    vec2 nuv = uv + noiseOffset;

    // Sampling the texture with noisy coordinates
    vec4 texColor = texture(samp, nuv);

    // Optionally, mix some smoke color (e.g., slight gray scaling)
    vec4 smokeColor = mix(texColor, vec4(0.6, 0.6, 0.6, 1.0), 0.2);

    // Output the final color
    color = smokeColor;
}
