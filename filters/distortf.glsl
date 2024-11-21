#version 330 core

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;
void main() {
    vec2 uv = tc;
    vec2 center = vec2(0.5, 0.5);
    vec2 offset = uv - center;

    float fractalFactor = sin(length(offset) * 10.0 + time_f * 2.0) * 0.1;
    vec2 fractalUV = uv + fractalFactor * normalize(offset);
    float grid = abs(sin(fractalUV.x * 50.0) * sin(fractalUV.y * 50.0));
    grid = step(0.7, grid);
    float angle = atan(offset.y, offset.x) + fractalFactor * sin(time_f);
    float radius = length(offset);
    vec2 swirlUV = center + radius * vec2(cos(angle), sin(angle));

    vec2 combinedUV = mix(swirlUV, fractalUV, 0.5);
    vec4 texColor = texture(samp, combinedUV);
    vec3 rainbow = vec3(0.5 + 0.5 * sin(time_f + texColor.r),
                        0.5 + 0.5 * sin(time_f + texColor.g + 2.0),
                        0.5 + 0.5 * sin(time_f + texColor.b + 4.0));

    vec3 finalColor = mix(texColor.rgb * rainbow, vec3(grid), 0.3);

    color = vec4(finalColor, texColor.a);
}

