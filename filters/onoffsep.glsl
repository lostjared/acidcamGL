#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    vec2 uv = tc;

    float lineWidth = 0.01 + 0.005 * sin(time_f * 2.0);
    float splitOffset = 0.05 * sin(time_f * 1.5);
    float linePosition = abs(fract(uv.y * 10.0 + time_f) - 0.5);
    float frameSep = mod(floor(uv.x * 5.0) + floor(uv.y * 5.0), 2.0);

    vec2 displacedUV = uv;
    if (frameSep == 0.0) {
        displacedUV.x += splitOffset;
    } else {
        displacedUV.x -= splitOffset;
    }

    vec4 texColor = texture(samp, displacedUV);

    if (linePosition < lineWidth) {
        texColor.rgb += vec3(1.0, 0.0, 0.0) * abs(sin(time_f)); // Highlight lines in red
    }

    color = texColor;
}

