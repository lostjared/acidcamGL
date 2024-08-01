#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

float pingPong(float x, float length) {
    float modVal = mod(x, length * 2.0);
    return modVal <= length ? modVal : length * 2.0 - modVal;
}

void main(void) {
    float time_t = pingPong(time_f, 5.0) + 2.0;
    float radius = sin(0.5 * time_t);
    vec2 center = vec2(iResolution.x / 2.0, iResolution.y / 2.0);
    vec2 texCoord = tc * iResolution;
    vec2 delta = texCoord - center;
    float dist = length(delta);
    float maxRadius = min(iResolution.x, iResolution.y) * radius;

    // Rotation logic
    float angle = time_f; // Rotate based on time
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    vec2 rotatedDelta = vec2(
        cosAngle * delta.x - sinAngle * delta.y,
        sinAngle * delta.x + cosAngle * delta.y
    );

    vec2 newTexCoord = texCoord;

    if (dist < maxRadius) {
        float scaleFactor = 1.0 - sqrt(dist / maxRadius);
        newTexCoord = center + rotatedDelta * scaleFactor;
    }

    newTexCoord = clamp(newTexCoord / iResolution, 0.0, 1.0);
    vec4 texColor = texture(samp, newTexCoord);
    
    color = texColor;
}
