#version 330 core
out vec4 color;
in vec2 tc;

uniform sampler2D samp;
uniform vec2 iResolution;
uniform float time_f;
vec3 hash3(vec2 p) {
    vec3 q = vec3(dot(p, vec2(127.1, 311.7)),
                  dot(p, vec2(269.5, 183.3)),
                  dot(p, vec2(419.2, 371.9)));
    return fract(sin(q) * 43758.5453);
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
    vec2 uv = tc * iResolution / vec2(iResolution.y);
    vec2 skewedUv = uv;
    skewedUv.x = (uv.x + uv.y) * 0.5;
    skewedUv.y = (uv.x - uv.y) * 0.5;
    float scale = abs(sin(time_f)) * 20.0 + 1.0;
    skewedUv *= scale;
    vec2 c = floor(skewedUv);
    vec2 checker = mod(c, 2.0);
    bool isEvenDiamond = (checker.x == checker.y);
    vec3 diamondColor = hash3(c);
    vec3 finalColor = isEvenDiamond ? diamondColor * 0.8 : diamondColor * 0.5;
    color = xor_RGB(vec4(finalColor, 1.0), texture(samp, tc));
    
}
