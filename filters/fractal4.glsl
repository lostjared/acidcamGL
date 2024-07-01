#version 330 core

in vec2 tc;
out vec4 color;
uniform float time_f;
uniform vec2 iResolution;
uniform sampler2D samp;

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec2 screenToComplex(vec2 screenPos, vec2 center, float zoom) {
    vec2 scale = vec2(zoom * (iResolution.x / iResolution.y), zoom);
    return center + (screenPos - iResolution * 0.5) * scale;
}

const int maxIterations = 300;

int mandelbrot(vec2 c) {
    vec2 z = vec2(0.0);
    int n = 0;
    for (int i = 0; i < maxIterations; i++) {
        z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;
        if (dot(z, z) > 4.0) break;
        n++;
    }
    return n;
}

vec4 xor_RGB(vec4 icolor, vec4 isourcex) {
    ivec4 isource = ivec4(isourcex * 255.0);
    ivec3 int_color;
    for(int i = 0; i < 3; ++i) {
        int_color[i] = int(255.0 * icolor[i]);
        int_color[i] ^= isource[i];
        if(int_color[i] > 255)
            int_color[i] %= 255;
        icolor[i] = float(int_color[i]) / 255.0;
    }
    return icolor;
}

vec2 interestingCenters(int index) {
    vec2 centers[5];
    centers[0] = vec2(-0.743643887037151, 0.13182590420533);
    centers[1] = vec2(-0.745428, 0.113009);
    centers[2] = vec2(-0.7453, 0.1127);
    centers[3] = vec2(-0.7435, 0.1318);
    centers[4] = vec2(-0.743644786, 0.1318252536);
    return centers[index % 5];
}

void main() {
    float zoomCycleSpeed = 0.045;
    float zoomIntensity = 20.0;
    float zoom = exp(-abs(sin(time_f * zoomCycleSpeed)) * zoomIntensity);
    
    int cycleIndex = int(floor(time_f / (3.14159 / zoomCycleSpeed)));
    vec2 center = interestingCenters(cycleIndex);

    vec2 c = screenToComplex(gl_FragCoord.xy, center, zoom * iResolution.y / iResolution.x);
    int n = mandelbrot(c);

    float colorIndex = rand(vec2(n, time_f));
    vec3 col = 0.5 + 0.5 * cos(6.28318 * (vec3(colorIndex) + vec3(0.0, 0.33, 0.67)));

    vec4 mandelbrotColor = n == maxIterations ? vec4(0.0, 0.0, 0.0, 1.0) : vec4(col, 1.0);
    
    vec4 textureColor = texture(samp, tc);

    color = xor_RGB(mandelbrotColor, textureColor);
}
