#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

void main(void) {
    vec2 uv = tc * iResolution / vec2(iResolution.y);
    vec2 center = vec2(0.5, 0.5) * iResolution / vec2(iResolution.y);
    vec2 pos = (uv - center) * 2.0;
    float angle = time_f;
    float cosA = cos(angle);
    float sinA = sin(angle);
    mat2 rotate = mat2(cosA, -sinA, sinA, cosA);
    pos = rotate * pos;
    float r = length(pos);
    float a = atan(pos.y, pos.x);
    float n = 5.0;
    float t = mod(a + 3.14159 / n, 2.0 * 3.14159 / n) * n / 2.0 / 3.14159;
    t = abs(t - 0.5);
    float starMask = smoothstep(0.02, 0.04, t + r * 0.5 - 0.3) * smoothstep(0.9, 0.7, r);
    vec4 texColor = texture(samp, tc);
    vec3 darkenedTexture = texColor.rgb * 0.5;
    vec3 finalColor = mix(darkenedTexture, texColor.rgb, starMask);

    float time_t = mod(time_f, 10);
    
    if(starMask < 0.5)
        color = texture(samp, sin(tc * cos(time_t *a )));
    else
        color = vec4(finalColor, 1.0);
}
