#version 330 core

in vec2 tc;
out vec4 color;
uniform float time_f;
uniform sampler2D samp;
uniform vec2 iResolution;

vec4 xor_RGB(vec4 icolor, vec4 isourcex) {
    ivec4 isource = ivec4(isourcex * 255);
    ivec3 int_color;
    for(int i = 0; i < 3; ++i) {
        int_color[i] = int(255 * icolor[i]);
        int_color[i] ^= isource[i];
        if(int_color[i] > 255)
            int_color[i] %= 255;
        icolor[i] = float(int_color[i]) / 255.0;
    }
    return icolor;
}

float hash(float n) {
    return fract(sin(n) * 43758.5453123);
}

vec2 random2(vec2 st) {
    st = vec2(dot(st, vec2(127.1, 311.7)), dot(st, vec2(269.5, 183.3)));
    return -1.0 + 2.0 * fract(sin(st) * 43758.5453123);
}

vec2 smoothRandom2(float t) {
    float t0 = floor(t);
    float t1 = t0 + 1.0;
    vec2 rand0 = random2(vec2(t0));
    vec2 rand1 = random2(vec2(t1));
    float mix_factor = fract(t);
    return mix(rand0, rand1, smoothstep(0.0, 1.0, mix_factor));
}

vec3 rainbow(float t) {
    t = fract(t);
    float r = abs(t * 6.0 - 3.0) - 1.0;
    float g = 2.0 - abs(t * 6.0 - 2.0);
    float b = 2.0 - abs(t * 6.0 - 4.0);
    return clamp(vec3(r, g, b), 0.0, 1.0);
}

void main(void) {
    vec2 normPos = (gl_FragCoord.xy / iResolution.xy) * 2.0 - 1.0;
    float dist = length(normPos);
    float phase = sin(dist * 10.0 - time_f * 4.0);
    vec2 tcAdjusted = tc + (normPos * 0.305 * phase);
    vec4 texColor = texture(samp, tcAdjusted);
    
    vec2 uv = (tc - 0.5) * 2.0;
    uv.x *= iResolution.x / iResolution.y;
    float time = time_f * 0.5;
    float sine1 = sin(uv.x * 10.0 + time) * cos(uv.y * 10.0 + time);
    float sine2 = sin(uv.y * 15.0 - time) * cos(uv.x * 15.0 - time);
    float sine3 = sin(sqrt(uv.x * uv.x + uv.y * uv.y) * 20.0 + time);
    float pattern = sine1 + sine2 + sine3;
    float colorIntensity = pattern * 0.5 + 0.5;
    vec3 psychedelicColor = vec3(
        sin(colorIntensity * 6.28318 + 0.0) * 0.5 + 0.5,
        sin(colorIntensity * 6.28318 + 2.09439) * 0.5 + 0.5,
        sin(colorIntensity * 6.28318 + 4.18879) * 0.5 + 0.5
    );
    vec4 finalColor = vec4(psychedelicColor, 1.0);
    vec4 xorColor = xor_RGB(finalColor, texColor);

    vec2 uv2 = tc * 2.0 - 1.0;
    uv2.y *= iResolution.y / iResolution.x;
    float wave = sin(uv2.x * 10.0 + time_f * 2.0) * 0.1;
    vec2 random_direction = smoothRandom2(time_f) * 0.5;
    float expand = 0.5 + 0.5 * sin(time_f * 2.0);
    vec2 spiral_uv = uv2 * expand + random_direction;
    float angle = atan(spiral_uv.y + wave, spiral_uv.x) + time_f * 2.0;
    vec3 rainbow_color = rainbow(angle / (2.0 * 3.14159));
    vec4 original_color = texture(samp, tc);
    vec3 blended_color = mix(original_color.rgb, rainbow_color, 0.5);
    float time_t = mod(time_f, 30.0);
    vec4 finalBlendedColor = vec4(sin(blended_color * time_t), original_color.a);

    float speed = 5.0;
    float amplitude = 0.03;
    float wavelength = 10.0;
    float ripple = sin(tc.x * wavelength + time_f * speed) * amplitude;
    ripple += sin(tc.y * wavelength + time_f * speed) * amplitude;
    vec2 rippleTC = tc + vec2(ripple, ripple);
    vec4 rippleColor = texture(samp, rippleTC);

    vec4 combinedColor = mix(xorColor, finalBlendedColor, 0.5);
    color = mix(combinedColor, rippleColor, 0.5);
}
