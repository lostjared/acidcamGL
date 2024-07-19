#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

void main() {
    vec2 uv = tc * 2.0 - 1.0;
    float angle = atan(uv.y, uv.x);
    float radius = length(uv);
    
    float spiral = angle + radius * 5.0 - time_f;
    float reflection = mod(spiral, 3.14159 * 2.0);
    
    vec2 spiralUV = vec2(cos(spiral), sin(spiral)) * radius + 0.5;
    vec2 reflectUV = vec2(cos(reflection), sin(reflection)) * radius + 0.5;

    vec4 gradientColor = vec4(0.5 + 0.5 * cos(time_f + radius * 10.0), 0.5 + 0.5 * sin(time_f + radius * 10.0), 0.5 + 0.5 * cos(time_f - radius * 10.0), 1.0);
    vec4 textureColor = texture(samp, reflectUV);
    
    color = mix(gradientColor, textureColor, 0.5);
}
