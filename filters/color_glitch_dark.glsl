#version 330

in vec2 tc;
out vec4 color;
uniform sampler2D samp;
uniform float time_f;
uniform vec2 iResolution;

vec3 rgb2hsb(vec3 c) {
    vec4 K = vec4(0.0, -1.0/3.0, 2.0/3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsb2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
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


void main() {
    vec2 uv = tc;
    vec4 texColor = texture(samp, uv);
    vec2 texelSize = 1.0 / iResolution;
    vec3 result = vec3(0.0);
    float weight[5];
    weight[0] = 0.227027;
    weight[1] = 0.1945946;
    weight[2] = 0.1216216;
    weight[3] = 0.054054;
    weight[4] = 0.016216;
    for (int i = -4; i <= 4; i++) {
        result += texture(samp, uv + vec2(float(i) * texelSize.x, 0.0)).rgb * weight[abs(i)];
    }
    for (int j = -4; j <= 4; j++) {
        result += texture(samp, uv + vec2(0.0, float(j) * texelSize.y)).rgb * weight[abs(j)];
    }
    vec3 hsb = rgb2hsb(result / 2.0);
    hsb.x += time_f * 0.1;
    hsb.y = 0.5;
    hsb.z = 0.9;
    vec3 rgb = hsb2rgb(hsb);
    color = vec4(rgb * 0.8 + 0.2, texColor.a);
    color = xor_RGB(sin(color * time_f) + 0.5, texture(samp, tc));
}
