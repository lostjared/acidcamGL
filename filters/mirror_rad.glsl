#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

float pingPong(float t, float l) {
    return abs(mod(t, 2.0 * l) - l);
}

void main() {
    vec2 uv = tc;
    float stretch = 1.0 + pingPong(time_f, 2.0) * 0.5;

    if (uv.x < 0.5) {
        uv.x = 0.5 - (0.5 - uv.x) * stretch;
        uv.x = 1.0 - uv.x;
    } else {
        uv.x = 0.5 + (uv.x - 0.5) * stretch;
    }

    float radius = 1.0;
    vec2 center = vec2(0.5, 0.5) * iResolution;
    vec2 texCoord = uv * iResolution;
    vec2 delta = texCoord - center;
    float dist = length(delta);
    float maxRadius = min(iResolution.x, iResolution.y) * radius;

    if (dist < maxRadius) {
        float scaleFactor = pingPong(time_f, 8.0) * (1.0 - pow(dist / maxRadius, 2.0));
        vec2 direction = normalize(delta);
        texCoord += direction * scaleFactor * 50.0;
    }

    uv = texCoord / iResolution;
    color = texture(samp, uv);
}
