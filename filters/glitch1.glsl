#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(void) {
    vec2 uv = tc;
    float offset = rand(uv * time_f) - 0.5;
    uv.x += offset * 0.05;
    float jumpOffset = fract(sin(time_f) * 4321.58);
    if(jumpOffset > 0.9) {
        uv.y += jumpOffset * 0.2;
    }

    vec4 texel = texture(samp, uv);
    vec2 redOffset = vec2(rand(uv + time_f * 0.1) * 0.02, 0.0);
    float red = texture(samp, uv + redOffset).r;
   vec2 gbOffset = vec2(rand(uv - time_f * 0.1) * 0.02, 0.0);
    float green = texture(samp, uv + gbOffset).g;
    float blue = texture(samp, uv - gbOffset).b;
    color = vec4(red, green, blue, texel.a);
}
