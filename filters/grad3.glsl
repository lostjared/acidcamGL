#version 330 core
out vec4 color;
in vec2 tc;
uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;
float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec4 xor_RGB(vec4 icolor, vec4 source) {
    ivec3 int_color;
    ivec4 isource = ivec4(source * 255);
    for(int i = 0; i < 3; ++i) {
        int_color[i] = int(255 * icolor[i]);
        int_color[i] = int_color[i]^isource[i];
        if(int_color[i] > 255)
            int_color[i] = int_color[i]%255;
        icolor[i] = float(int_color[i])/255;
    }
    return icolor;
}

void main(void) {
    vec2 uv = tc;
    vec2 warp = uv + vec2(
        sin(uv.y * 10.0 + time_f) * 0.1,
        sin(uv.x * 20.0 + time_f) * 0.1
    );
    vec3 colorShift = vec3(
        0.2 * sin(time_f * 0.2) + 0.8,
        0.4 * sin(time_f * 0.4 + 6.0) + 0.5,
        0.8 * sin(time_f * 0.6 + 4.0) + 0.5
    );
    float time_t = mod(time_f, 50);
    float feedback = rand(uv + time_f);
    vec2 feedbackUv = warp;
    vec4 texColor = texture(samp, feedbackUv);
    vec3 finalColor = texColor.rgb + colorShift;
    color = sin(vec4(finalColor, texColor.a) * (time_f * 0.5));
}
