#version 330 core
out vec4 color;
in vec2 tc;
uniform sampler2D samp;
uniform sampler2D mat_samp;
uniform float time_f;
uniform vec2 iResolution;

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
    vec2 center = iResolution * 0.5;
    float maxRadius = length(iResolution * 0.5);
    float radius = maxRadius * (sin(time_f * 0.5) * 0.5 + 0.5);
    vec2 normalizedCoord = gl_FragCoord.xy - center;
    normalizedCoord.x /= iResolution.x;
    normalizedCoord.y /= iResolution.y;
    float dist = length(normalizedCoord);
    if (dist < radius / maxRadius) {
        color = xor_RGB(texture(samp, tc), texture(mat_samp, tc));
    } else {
        color = texture(samp, tc);
    }
}

