#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

void main(void) {
    float radius = 1.0;
    vec2 center = iResolution * 0.5;
    vec2 texCoord = tc * iResolution;
    vec2 delta = texCoord - center;
    float dist = length(delta);
    float maxRadius = min(iResolution.x, iResolution.y) * radius;

    if (dist < maxRadius) {
        float scaleFactor = 1.0 - pow(dist / maxRadius, 2.0);
        vec2 direction = normalize(delta);
        float offsetR = 0.015;
        float offsetG = 0.0;
        float offsetB = -0.015;

        vec2 texCoordR = center + delta * scaleFactor + direction * offsetR * maxRadius;
        vec2 texCoordG = center + delta * scaleFactor + direction * offsetG * maxRadius;
        vec2 texCoordB = center + delta * scaleFactor + direction * offsetB * maxRadius;

        texCoordR /= iResolution;
        texCoordG /= iResolution;
        texCoordB /= iResolution;
        texCoordR = clamp(texCoordR, 0.0, 1.0);
        texCoordG = clamp(texCoordG, 0.0, 1.0);
        texCoordB = clamp(texCoordB, 0.0, 1.0);
        float r = texture(samp, texCoordR).r;
        float g = texture(samp, texCoordG).g;
        float b = texture(samp, texCoordB).b;
        color = vec4(r, g, b, 1.0);
    } else {
        vec2 newTexCoord = clamp(tc, 0.0, 1.0);
        color = texture(samp, newTexCoord);
    }
}

