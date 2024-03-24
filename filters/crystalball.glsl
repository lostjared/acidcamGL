#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

void main(void) {
    float aspectRatio = iResolution.x / iResolution.y;
    vec2 correctedCenter = vec2(0.5, 0.5 / aspectRatio);
    vec2 adjustedTC = tc;
    adjustedTC.y /= aspectRatio;
    vec2 toCenter = adjustedTC - correctedCenter;
    float dist = length(toCenter) * aspectRatio;
    float zoomRange = 0.1 + (0.5 + 0.5 * sin(time_f * 2.0)) * 6.0;
    float dynamicRadius = 0.2 * zoomRange;
    if (dist < dynamicRadius) {
        float refractStrength = 0.5 * zoomRange;
        vec2 refractTC = toCenter * (1.0 - refractStrength * sqrt(1.0 - (dist / dynamicRadius))) / dist;
        refractTC.y *= aspectRatio;
        color = texture(samp, correctedCenter + refractTC);
    } else {
        color = texture(samp, adjustedTC);
    }
}
