#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

// Function to generate a random number based on a seed
float rand(float seed) {
    return fract(sin(seed) * 43758.5453123);
}

// Function to generate a random position based on a time-based seed
vec2 randomPos(float seed) {
    return vec2(rand(seed), rand(seed + 1.0));
}

// Function to expand and contract position based on time
vec2 expand(vec2 pos, vec2 center) {
    float t = mod(time_f, 4.0);
    float scale = abs(sin(t * 3.14159 / 2.0));
    return mix(center, pos, scale);
}

void main(void) {
    // Generate a random position based on the current cycle
    float cycle = floor(time_f / 4.0);
    vec2 center = randomPos(cycle);
    
    // Expand and contract around the random position
    vec2 pos = expand(tc, center);
    
    // Sample the texture at the modified position
    vec4 tcolor = texture(samp, pos);
    color = tcolor;
}
