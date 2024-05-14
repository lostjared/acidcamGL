#version 330 core

in vec2 tc;
out vec4 color;

uniform sampler2D samp;
uniform float time_f;

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec4 getColor(void) {
    vec4 ctx = texture(samp, tc);
    float adjusted_time = time_f * 0.1;

    float hue = mod(tc.x + adjusted_time, 1.0);
    vec3 hsvColor = vec3(hue, 1.0, 1.0);
    vec3 smoothColor = hsv2rgb(hsvColor);
    ctx.rgb = mix(ctx.rgb, smoothColor, 0.5);
    return ctx;
}

void main(void)
{
    vec4 color1 = getColor();
    vec2 uv = tc * 2.0 - 1.0;
    float t = time_f * 0.5;
    float glitchX = sin(uv.y * 10.0 + t) * 0.1;
    float glitchY = cos(uv.x * 10.0 + t) * 0.1;
    vec2 glitchUV = vec2(uv.x + glitchX, uv.y + glitchY);
    float r = length(glitchUV);
    float a = atan(glitchUV.y, glitchUV.x);
    float radius = sin(t + r * 5.0) * 0.5 + 0.5;
    float angle = a + t + sin(r * 20.0 + t) * 0.1;
    vec2 distorted_uv = vec2(cos(angle), sin(angle)) * radius + 0.5;

    vec3 col = texture(samp, distorted_uv).rgb;
    col = mix(col, color1.rgb, 0.8);
    
    col = mix(col, vec3(1.0, 0.0, 0.0), 0.5 * sin(t + length(uv) * 5.0));

    color = vec4(col, 1.0);
}

