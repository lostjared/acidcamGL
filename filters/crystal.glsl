#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;

void main(void) {
    float radius = 0.5; // Example hardcoded radius
    vec2 center = vec2(iResolution.x / 2.0, iResolution.y / 2.0); // Hardcoded center to the middle of the screen

    vec2 texCoord = tc * iResolution;
    vec2 delta = texCoord - center;
    float dist = length(delta);
    float maxRadius = min(iResolution.x, iResolution.y) * radius;

    vec2 newTexCoord = texCoord;

    if (dist < maxRadius) {
        float scaleFactor = 1.0 - sqrt(dist / maxRadius);
        newTexCoord = center + delta * scaleFactor;
    }

    newTexCoord = clamp(newTexCoord / iResolution, 0.0, 1.0);
    vec4 texColor = texture(samp, newTexCoord);
    
    color = texColor;
}
